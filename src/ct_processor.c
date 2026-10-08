#include "ct_processor.h"
#include "lang.h"

#ifdef _WIN32
#include <io.h>
#define ACCESS_FUNC _access
#else
#include <unistd.h>
#define ACCESS_FUNC access
#endif

/* Case-insensitive string comparison compatible with ANSI C / Win95+ */
static int ct_stricmp(const char* s1, const char* s2) {
    while (*s1 && *s2) {
        int c1 = tolower((unsigned char)*s1);
        int c2 = tolower((unsigned char)*s2);
        if (c1 != c2) return c1 - c2;
        s1++;
        s2++;
    }
    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}

/* Trim leading and trailing whitespace */
static char* ct_trim(char* s) {
    char* end;
    while (isspace((unsigned char)*s)) s++;
    if (*s == 0) return s;
    end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return s;
}

int ct_parse_pointer(const char* input_str, CTPointer* out_ptr) {
    char buf[512];
    char* p;
    char* sign_pos = NULL;

    if (!input_str || !out_ptr) return 0;
    memset(out_ptr, 0, sizeof(CTPointer));

    strncpy(buf, input_str, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    p = ct_trim(buf);
    if (!*p) return 0;

    /* Check if module is quoted, e.g., "pcsx2-qt.exe"+031FD968 */
    if (*p == '"') {
        char* quote_end = strchr(p + 1, '"');
        if (quote_end) {
            size_t mod_len = (size_t)(quote_end - (p + 1));
            if (mod_len >= sizeof(out_ptr->module_name)) {
                mod_len = sizeof(out_ptr->module_name) - 1;
            }
            strncpy(out_ptr->module_name, p + 1, mod_len);
            out_ptr->module_name[mod_len] = '\0';
            out_ptr->has_module = 1;

            p = quote_end + 1;
            while (isspace((unsigned char)*p)) p++;
        }
    } else {
        /* Check if there's a '+' or '-' after non-space characters that indicates a module */
        char* plus = strchr(p, '+');
        char* minus = strchr(p, '-');
        if (plus && (!minus || plus < minus)) {
            sign_pos = plus;
        } else if (minus) {
            sign_pos = minus;
        }

        /* If '+' or '-' found and not at index 0, everything before it is module name */
        if (sign_pos && sign_pos > p) {
            size_t mod_len = (size_t)(sign_pos - p);
            if (mod_len >= sizeof(out_ptr->module_name)) {
                mod_len = sizeof(out_ptr->module_name) - 1;
            }
            strncpy(out_ptr->module_name, p, mod_len);
            out_ptr->module_name[mod_len] = '\0';
            ct_trim(out_ptr->module_name);
            out_ptr->has_module = 1;
            p = sign_pos;
        }
    }

    /* Check sign before hex offset */
    while (isspace((unsigned char)*p)) p++;
    if (*p == '+') {
        out_ptr->is_negative = 0;
        p++;
    } else if (*p == '-') {
        out_ptr->is_negative = 1;
        p++;
    }

    while (isspace((unsigned char)*p)) p++;

    /* Skip optional 0x or 0X */
    if (p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) {
        p += 2;
    }

    if (!*p) return 0;

    /* Parse hexadecimal offset */
    char* endptr = NULL;
    out_ptr->offset = (uint64_t)strtoull(p, &endptr, 16);
    if (endptr == p) return 0; /* No valid hex digits */

    return 1;
}

int64_t ct_calculate_delta(const CTPointer* ptr_ori, const CTPointer* ptr_rep) {
    int64_t ori_val = (int64_t)ptr_ori->offset * (ptr_ori->is_negative ? -1 : 1);
    int64_t rep_val = (int64_t)ptr_rep->offset * (ptr_rep->is_negative ? -1 : 1);
    return rep_val - ori_val;
}

static int file_exists(const char* path) {
    return (ACCESS_FUNC(path, 0) == 0);
}

int ct_generate_output_path(const char* input_path, const char* base_dir, char* out_path, size_t out_path_size) {
    char filename[MAX_PATH];
    char basename[MAX_PATH];
    char ext[32];
    const char* last_sep;
    const char* dot;

    if (!input_path || !out_path || out_path_size == 0) return 0;

    /* Extract file name from input_path */
    last_sep = strrchr(input_path, '\\');
    if (!last_sep) last_sep = strrchr(input_path, '/');
    if (last_sep) {
        strncpy(filename, last_sep + 1, sizeof(filename) - 1);
    } else {
        strncpy(filename, input_path, sizeof(filename) - 1);
    }
    filename[sizeof(filename) - 1] = '\0';

    /* Separate basename and extension */
    dot = strrchr(filename, '.');
    if (dot) {
        size_t b_len = (size_t)(dot - filename);
        strncpy(basename, filename, b_len);
        basename[b_len] = '\0';
        strncpy(ext, dot, sizeof(ext) - 1);
        ext[sizeof(ext) - 1] = '\0';
    } else {
        strncpy(basename, filename, sizeof(basename) - 1);
        basename[sizeof(basename) - 1] = '\0';
        ext[0] = '\0';
    }

    /* Build candidate path: base_dir\basename_modified.ext */
    if (base_dir && *base_dir) {
        size_t len = strlen(base_dir);
        int has_slash = (base_dir[len - 1] == '\\' || base_dir[len - 1] == '/');
        if (has_slash) {
            snprintf(out_path, out_path_size, "%s%s_modified%s", base_dir, basename, ext);
        } else {
            snprintf(out_path, out_path_size, "%s\\%s_modified%s", base_dir, basename, ext);
        }
    } else {
        snprintf(out_path, out_path_size, "%s_modified%s", basename, ext);
    }

    /* If already exists, increment counter: (1), (2), ... */
    if (file_exists(out_path)) {
        int index = 1;
        while (index < 1000) {
            if (base_dir && *base_dir) {
                size_t len = strlen(base_dir);
                int has_slash = (base_dir[len - 1] == '\\' || base_dir[len - 1] == '/');
                if (has_slash) {
                    snprintf(out_path, out_path_size, "%s%s_modified (%d)%s", base_dir, basename, index, ext);
                } else {
                    snprintf(out_path, out_path_size, "%s\\%s_modified (%d)%s", base_dir, basename, index, ext);
                }
            } else {
                snprintf(out_path, out_path_size, "%s_modified (%d)%s", basename, index, ext);
            }

            if (!file_exists(out_path)) {
                break;
            }
            index++;
        }
    }

    return 1;
}

int ct_process_file(const char* input_path,
                    const char* ptr_ori_str,
                    const char* ptr_rep_str,
                    const char* base_output_dir,
                    CTProcessResult* result) {
    CTPointer ptr_ori, ptr_rep;
    FILE* fin = NULL;
    FILE* fout = NULL;
    char line[16384];

    if (!result) return 0;
    memset(result, 0, sizeof(CTProcessResult));

    if (!input_path || !*input_path) {
        snprintf(result->error_message, sizeof(result->error_message), "%s", _T("err_input_missing"));
        return 0;
    }

    if (!ct_parse_pointer(ptr_ori_str, &ptr_ori)) {
        snprintf(result->error_message, sizeof(result->error_message), "%s", _T("err_invalid_ori"));
        return 0;
    }

    if (!ct_parse_pointer(ptr_rep_str, &ptr_rep)) {
        snprintf(result->error_message, sizeof(result->error_message), "%s", _T("err_invalid_rep"));
        return 0;
    }

    /* Check module consistency */
    if (ptr_ori.has_module && ptr_rep.has_module) {
        if (ct_stricmp(ptr_ori.module_name, ptr_rep.module_name) != 0) {
            snprintf(result->error_message, sizeof(result->error_message),
                     _T("err_module_mismatch"), ptr_ori.module_name, ptr_rep.module_name);
            return 0;
        }
        strncpy(result->target_module, ptr_ori.module_name, sizeof(result->target_module) - 1);
    } else if (ptr_ori.has_module) {
        strncpy(result->target_module, ptr_ori.module_name, sizeof(result->target_module) - 1);
    } else if (ptr_rep.has_module) {
        strncpy(result->target_module, ptr_rep.module_name, sizeof(result->target_module) - 1);
    } else {
        result->target_module[0] = '\0';
    }

    result->delta = ct_calculate_delta(&ptr_ori, &ptr_rep);

    if (!ct_generate_output_path(input_path, base_output_dir, result->output_filepath, sizeof(result->output_filepath))) {
        snprintf(result->error_message, sizeof(result->error_message), "%s", _T("err_output_path"));
        return 0;
    }

    fin = fopen(input_path, "r");
    if (!fin) {
        snprintf(result->error_message, sizeof(result->error_message), _T("err_open_input"), input_path);
        return 0;
    }

    fout = fopen(result->output_filepath, "w");
    if (!fout) {
        fclose(fin);
        snprintf(result->error_message, sizeof(result->error_message), _T("err_open_output"), result->output_filepath);
        return 0;
    }

    while (fgets(line, sizeof(line), fin)) {
        char* tag_start = strstr(line, "<Address>");
        char* tag_end = tag_start ? strstr(tag_start, "</Address>") : NULL;

        if (tag_start && tag_end) {
            result->pointers_found++;

            /* Extract content inside <Address>...</Address> */
            char addr_content[1024];
            size_t content_len = (size_t)(tag_end - (tag_start + 9));
            if (content_len >= sizeof(addr_content)) {
                content_len = sizeof(addr_content) - 1;
            }
            strncpy(addr_content, tag_start + 9, content_len);
            addr_content[content_len] = '\0';

            CTPointer current_ptr;
            if (ct_parse_pointer(addr_content, &current_ptr)) {
                int module_matches = 0;

                if (result->target_module[0] != '\0') {
                    if (current_ptr.has_module && ct_stricmp(current_ptr.module_name, result->target_module) == 0) {
                        module_matches = 1;
                    }
                } else {
                    /* If no target module specified, all pointers match */
                    module_matches = 1;
                }

                if (module_matches) {
                    /* Calculate new offset */
                    int64_t current_signed = (int64_t)current_ptr.offset * (current_ptr.is_negative ? -1 : 1);
                    int64_t new_val = current_signed + result->delta;

                    /* Measure original hex width for nice zero-padded formatting */
                    /* Find the hex digits in addr_content */
                    const char* p_hex = addr_content;
                    const char* plus_or_minus = strpbrk(addr_content, "+-");
                    if (plus_or_minus) {
                        p_hex = plus_or_minus + 1;
                    } else if (addr_content[0] == '"') {
                        char* q2 = strchr(addr_content + 1, '"');
                        if (q2) p_hex = q2 + 1;
                    }
                    while (isspace((unsigned char)*p_hex)) p_hex++;
                    if (p_hex[0] == '0' && (p_hex[1] == 'x' || p_hex[1] == 'X')) p_hex += 2;

                    int orig_width = 8;
                    int hex_digits = 0;
                    while (isxdigit((unsigned char)p_hex[hex_digits])) {
                        hex_digits++;
                    }
                    if (hex_digits > orig_width) orig_width = hex_digits;

                    /* Build new address string */
                    char new_addr_str[512];
                    if (current_ptr.has_module) {
                        if (new_val >= 0) {
                            snprintf(new_addr_str, sizeof(new_addr_str), "\"%.250s\"+%0*I64X",
                                     current_ptr.module_name, orig_width, (unsigned __int64)new_val);
                        } else {
                            snprintf(new_addr_str, sizeof(new_addr_str), "\"%.250s\"-%0*I64X",
                                     current_ptr.module_name, orig_width, (unsigned __int64)(-new_val));
                        }
                    } else {
                        if (new_val >= 0) {
                            snprintf(new_addr_str, sizeof(new_addr_str), "%0*I64X",
                                     orig_width, (unsigned __int64)new_val);
                        } else {
                            snprintf(new_addr_str, sizeof(new_addr_str), "-%0*I64X",
                                     orig_width, (unsigned __int64)(-new_val));
                        }
                    }

                    /* Write line with replaced <Address> */
                    *tag_start = '\0';
                    fputs(line, fout);
                    fputs("<Address>", fout);
                    fputs(new_addr_str, fout);
                    fputs(tag_end, fout);

                    result->pointers_modified++;
                    continue;
                }
            }
        }

        /* If no replacement made, write original line */
        fputs(line, fout);
    }

    fclose(fin);
    fclose(fout);

    result->success = 1;
    return 1;
}
