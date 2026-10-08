#include "compiler_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <io.h>
#define FILE_EXISTS(p) (_access(p, 0) == 0)
#else
#include <unistd.h>
#define FILE_EXISTS(p) (access(p, 0) == 0)
#endif

static long get_file_size(const char* filename) {
    FILE* f = fopen(filename, "rb");
    if (!f) return -1;
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fclose(f);
    return size;
}

int compiler_detect_environment(CompilerEnvironment* env) {
    if (!env) return 0;
    memset(env, 0, sizeof(CompilerEnvironment));

    /* Candidate paths for MinGW32 (i686) */
    const char* cand32[] = {
        "D:\\msys64\\mingw32\\bin\\gcc.exe",
        "C:\\msys64\\mingw32\\bin\\gcc.exe",
        "D:\\mingw32\\bin\\gcc.exe",
        "C:\\mingw32\\bin\\gcc.exe",
        NULL
    };

    /* Candidate paths for MinGW64 (x86_64) */
    const char* cand64[] = {
        "D:\\c_projects\\compilers\\mingw64\\bin\\gcc.exe",
        "D:\\msys64\\mingw64\\bin\\gcc.exe",
        "D:\\msys64\\ucrt64\\bin\\gcc.exe",
        "C:\\msys64\\mingw64\\bin\\gcc.exe",
        "C:\\msys64\\ucrt64\\bin\\gcc.exe",
        NULL
    };

    for (int i = 0; cand32[i]; i++) {
        if (FILE_EXISTS(cand32[i])) {
            strncpy(env->mingw32_gcc, cand32[i], sizeof(env->mingw32_gcc) - 1);
            env->has_mingw32 = 1;
            break;
        }
    }

    for (int i = 0; cand64[i]; i++) {
        if (FILE_EXISTS(cand64[i])) {
            strncpy(env->mingw64_gcc, cand64[i], sizeof(env->mingw64_gcc) - 1);
            env->has_mingw64 = 1;
            break;
        }
    }

    /* Fallback if mingw32 not found, try fallback to gcc in PATH */
    if (!env->has_mingw32) {
        strncpy(env->mingw32_gcc, "gcc.exe", sizeof(env->mingw32_gcc) - 1);
        env->has_mingw32 = 1; /* Best effort fallback */
    }

    if (!env->has_mingw64) {
        if (env->has_mingw32 && strcmp(env->mingw32_gcc, "gcc.exe") != 0) {
            /* Can reuse 32-bit gcc or PATH gcc */
            strncpy(env->mingw64_gcc, env->mingw32_gcc, sizeof(env->mingw64_gcc) - 1);
            env->has_mingw64 = 1;
        } else {
            strncpy(env->mingw64_gcc, "gcc.exe", sizeof(env->mingw64_gcc) - 1);
            env->has_mingw64 = 1;
        }
    }

    return 1;
}

void compiler_print_targets(void) {
    printf("======================================================================\n");
    printf(" DAFTAR TARGET SISTEM OPERASI & PENGATURAN KOMPILER PE\n");
    printf("======================================================================\n");
    for (int i = 0; i < TARGET_COUNT; i++) {
        const OSTargetDef* t = &G_TARGETS[i];
        printf("[%d] Target OS  : %s\n", i + 1, t->os_name);
        printf("    Output Exe : %s\n", t->output_exe);
        printf("    Toolchain  : %s\n", (t->toolchain == TOOLCHAIN_MINGW32) ? "MinGW32 (i686 x86)" : "MinGW64 (x86_64)");
        printf("    Arch Flags : %s\n", t->arch_flags);
        printf("    Subsystem  : %s (%s)\n", t->subsystem_flags, t->os_version_flags);
        printf("    Keterangan : %s\n", t->desc);
        printf("----------------------------------------------------------------------\n");
    }
}

int compiler_build_target(const CompilerEnvironment* env, OSTargetID target_id) {
    if (target_id < 0 || target_id >= TARGET_COUNT) return 0;
    const OSTargetDef* t = &G_TARGETS[target_id];

    const char* gcc_path = (t->toolchain == TOOLCHAIN_MINGW32) ? env->mingw32_gcc : env->mingw64_gcc;

    printf("\n>>> [BUILDING] %s -> %s ...\n", t->os_name, t->output_exe);
    printf("    Compiler  : %s\n", gcc_path);
    printf("    Arsitektur: %s\n", t->arch_flags);
    printf("    Subsystem : %s %s\n", t->subsystem_flags, t->os_version_flags);

    /* Extract compiler directory for PATH setup */
    char comp_dir[MAX_PATH] = "";
    const char* last_sep = strrchr(gcc_path, '\\');
    if (!last_sep) last_sep = strrchr(gcc_path, '/');
    if (last_sep) {
        size_t len = (size_t)(last_sep - gcc_path);
        strncpy(comp_dir, gcc_path, len);
        comp_dir[len] = '\0';
    }

    const char* res_obj = (t->toolchain == TOOLCHAIN_MINGW32) ? "src/resource_32.o" : "src/resource_64.o";

    char cmd[4096];
    if (comp_dir[0] != '\0') {
        snprintf(cmd, sizeof(cmd),
                 "cmd.exe /c \"set PATH=%s;%%PATH%% && \"%s\" -O2 %s %s %s "
                 "-static -static-libgcc "
                 "src/main.c src/gui.c src/ct_processor.c src/lang.c %s "
                 "-lcomdlg32 -lshell32 -lgdi32 -luser32 -lkernel32 "
                 "-o \"%s\"\"",
                 comp_dir, gcc_path, t->arch_flags, t->subsystem_flags, t->os_version_flags,
                 res_obj, t->output_exe);
    } else {
        snprintf(cmd, sizeof(cmd),
                 "cmd.exe /c \"\"%s\" -O2 %s %s %s "
                 "-static -static-libgcc "
                 "src/main.c src/gui.c src/ct_processor.c src/lang.c %s "
                 "-lcomdlg32 -lshell32 -lgdi32 -luser32 -lkernel32 "
                 "-o \"%s\"\"",
                 gcc_path, t->arch_flags, t->subsystem_flags, t->os_version_flags,
                 res_obj, t->output_exe);
    }

    int ret = system(cmd);
    if (ret != 0) {
        printf("    [ERROR] Kompilasi gagal untuk %s! (Exit code: %d)\n", t->os_name, ret);
        return 0;
    }

    long sz = get_file_size(t->output_exe);
    if (sz < 0) {
        printf("    [ERROR] File output tidak ditemukan: %s\n", t->output_exe);
        return 0;
    }

    printf("    [SUKSES] %s berhasil dibuat! Ukuran file: %ld bytes (%.1f KB)\n",
           t->output_exe, sz, (double)sz / 1024.0);
    return 1;
}

int compiler_build_all(const CompilerEnvironment* env) {
    int success_count = 0;
    printf("\n======================================================================\n");
    printf(" MEMULAI PROSES KOMPILASI MULTI-OS TARGET (WIN95 s/d WIN10)\n");
    printf("======================================================================\n");

    for (int i = 0; i < TARGET_COUNT; i++) {
        if (compiler_build_target(env, (OSTargetID)i)) {
            success_count++;
        }
    }

    printf("\n======================================================================\n");
    printf(" RINGKASAN KOMPILASI: %d dari %d target berhasil dikompilasi!\n", success_count, TARGET_COUNT);
    printf("======================================================================\n");

    return (success_count == TARGET_COUNT);
}
