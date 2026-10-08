#include "lang.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    const char *key;
    const char *val;
} BuiltinLangEntry;

/* Indonesian dictionary */
static const BuiltinLangEntry G_LANG_ID[] = {
    {"app_title", "Cheat Engine Pointer Adapter (Win95/98/XP/7/10)"},
    {"file_label", "Pilih File Cheat Engine (.CT):"},
    {"browse_btn", "Browse..."},
    {"ptr_ori_label", "Pointer Original (contoh: \"pcsx2-qt.exe\"+0317C7E8):"},
    {"ptr_rep_label", "Pointer Pengganti (contoh: \"pcsx2-qt.exe\"+031FD968):"},
    {"process_btn", "Hitung Selisih && Refaktor Pointer"},
    {"status_label", "Status & Log Aktivitas:"},
    {"ready_log", "Siap memproses. Anda juga dapat melakukan Drag && Drop file .CT ke jendela ini.\r\n"},
    {"warn_select_file", "Silakan pilih file Cheat Engine (.CT) terlebih dahulu!"},
    {"warn_fill_ori", "Silakan isi Pointer Original!"},
    {"warn_fill_rep", "Silakan isi Pointer Pengganti!"},
    {"warn_title", "Peringatan"},
    {"error_title", "Gagal Memproses"},
    {"success_title", "Proses Berhasil"},
    {"log_start", "--- Memulai Proses Refaktor Pointer ---\r\n"},
    {"log_input_file", "File Input"},
    {"log_ptr_ori", "Pointer Ori"},
    {"log_ptr_rep", "Pointer Pengganti"},
    {"log_result_title", "Hasil Perhitungan:"},
    {"log_target_module", "- Modul Target      :"},
    {"log_delta", "- Selisih (Delta)   :"},
    {"log_total_found", "- Total Tag Address :"},
    {"log_pointers_modified", "- Pointer Diubah    :"},
    {"log_output_file", "- File Hasil        :"},
    {"log_success", "[SUKSES] File .CT baru berhasil disimpan!\r\n"},
    {"dialog_finished", "Selesai!\r\n\r\nSelisih pointer: %s\r\nPointer direfaktor: %d dari %d\r\n\r\nFile disimpan ke:\r\n%s"},
    {"lang_label", "Bahasa:"},
    {"all_modules", "(Semua modul)"},
    {"unit_found", "ditemukan"},
    {"unit_modified", "berhasil direfaktor"},
    {"decimal_word", "desimal"},
    {"err_input_missing", "File input .CT belum dipilih."},
    {"err_invalid_ori", "Format Pointer Original tidak valid!"},
    {"err_invalid_rep", "Format Pointer Pengganti tidak valid!"},
    {"err_module_mismatch", "Nama modul berbeda: '%.200s' vs '%.200s'"},
    {"err_output_path", "Gagal membuat path file output."},
    {"err_open_input", "Gagal membuka file input: %s"},
    {"err_open_output", "Gagal membuat file output: %s"},
    {NULL, NULL}
};

/* English dictionary */
static const BuiltinLangEntry G_LANG_EN[] = {
    {"app_title", "Cheat Engine Pointer Adapter (Win95/98/XP/7/10)"},
    {"file_label", "Select Cheat Engine Table (.CT):"},
    {"browse_btn", "Browse..."},
    {"ptr_ori_label", "Original Pointer (e.g. \"pcsx2-qt.exe\"+0317C7E8):"},
    {"ptr_rep_label", "Replacement Pointer (e.g. \"pcsx2-qt.exe\"+031FD968):"},
    {"process_btn", "Calculate Delta && Refactor Pointers"},
    {"status_label", "Status & Activity Log:"},
    {"ready_log", "Ready. You can also drag and drop a .CT file into this window.\r\n"},
    {"warn_select_file", "Please select a Cheat Engine Table (.CT) file first!"},
    {"warn_fill_ori", "Please enter the Original Pointer!"},
    {"warn_fill_rep", "Please enter the Replacement Pointer!"},
    {"warn_title", "Warning"},
    {"error_title", "Processing Failed"},
    {"success_title", "Process Successful"},
    {"log_start", "--- Starting Pointer Refactoring Process ---\r\n"},
    {"log_input_file", "Input File"},
    {"log_ptr_ori", "Original Pointer"},
    {"log_ptr_rep", "Replacement Pointer"},
    {"log_result_title", "Calculation Results:"},
    {"log_target_module", "- Target Module     :"},
    {"log_delta", "- Delta (Difference):"},
    {"log_total_found", "- Total Address Tags:"},
    {"log_pointers_modified", "- Pointers Refactored:"},
    {"log_output_file", "- Output File       :"},
    {"log_success", "[SUCCESS] New .CT file successfully saved!\r\n"},
    {"dialog_finished", "Finished!\r\n\r\nPointer delta: %s\r\nRefactored pointers: %d of %d\r\n\r\nSaved to:\r\n%s"},
    {"lang_label", "Language:"},
    {"all_modules", "(All modules)"},
    {"unit_found", "found"},
    {"unit_modified", "successfully refactored"},
    {"decimal_word", "decimal"},
    {"err_input_missing", "No input .CT file selected."},
    {"err_invalid_ori", "Invalid Original Pointer format!"},
    {"err_invalid_rep", "Invalid Replacement Pointer format!"},
    {"err_module_mismatch", "Module names do not match: '%.200s' vs '%.200s'"},
    {"err_output_path", "Failed to generate output file path."},
    {"err_open_input", "Failed to open input file: %s"},
    {"err_open_output", "Failed to create output file: %s"},
    {NULL, NULL}
};

static char g_current_lang[8] = "id";

void lang_init(const char *lang_code) {
    if (lang_code && *lang_code) {
        lang_set(lang_code);
    } else {
        lang_set("id");
    }
}

void lang_set(const char *lang_code) {
    if (!lang_code) return;
    if (strcmp(lang_code, "en") == 0 || strcmp(lang_code, "EN") == 0) {
        strncpy(g_current_lang, "en", sizeof(g_current_lang) - 1);
    } else {
        strncpy(g_current_lang, "id", sizeof(g_current_lang) - 1);
    }
    g_current_lang[sizeof(g_current_lang) - 1] = '\0';
}

const char *lang_get_current(void) {
    return g_current_lang;
}

const char *lang_str(const char *key) {
    if (!key) return "";

    const BuiltinLangEntry *dict = (strcmp(g_current_lang, "en") == 0) ? G_LANG_EN : G_LANG_ID;

    for (int i = 0; dict[i].key != NULL; i++) {
        if (strcmp(dict[i].key, key) == 0) {
            return dict[i].val;
        }
    }

    /* Fallback to other language or raw key */
    const BuiltinLangEntry *fallback = (dict == G_LANG_EN) ? G_LANG_ID : G_LANG_EN;
    for (int i = 0; fallback[i].key != NULL; i++) {
        if (strcmp(fallback[i].key, key) == 0) {
            return fallback[i].val;
        }
    }

    return key;
}
