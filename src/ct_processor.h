#ifndef CT_PROCESSOR_H
#define CT_PROCESSOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#ifdef _WIN32
#include <windows.h>
#else
#ifndef MAX_PATH
#define MAX_PATH 260
#endif
#endif

typedef struct {
    char module_name[MAX_PATH];
    int has_module;
    int is_negative;
    uint64_t offset;
} CTPointer;

typedef struct {
    int success;
    int pointers_found;
    int pointers_modified;
    int64_t delta;
    char target_module[MAX_PATH];
    char output_filepath[MAX_PATH];
    char error_message[512];
} CTProcessResult;

/*
 * Parse a pointer string like:
 *   "pcsx2-qt.exe"+031FD968
 *   pcsx2-qt.exe+031FD968
 *   031FD968
 *   +031FD968
 */
int ct_parse_pointer(const char* str, CTPointer* out_ptr);

/*
 * Calculate delta = ptr_replacement.offset - ptr_original.offset
 */
int64_t ct_calculate_delta(const CTPointer* ptr_ori, const CTPointer* ptr_rep);

/*
 * Generate safe output filename in root directory (same folder as exe, or file dir)
 * Example: original "FFX International Jap.CT"
 * -> "FFX International Jap_modified.CT"
 * If exists: "FFX International Jap_modified (1).CT", etc.
 */
int ct_generate_output_path(const char* input_path, const char* base_dir, char* out_path, size_t out_path_size);

/*
 * Process the entire .CT file and save to output path.
 */
int ct_process_file(const char* input_path,
                    const char* ptr_ori_str,
                    const char* ptr_rep_str,
                    const char* base_output_dir,
                    CTProcessResult* result);

#endif /* CT_PROCESSOR_H */
