#include "compiler_manager.h"
#include <stdio.h>
#include <string.h>

static void print_banner(void) {
    printf("======================================================================\n");
    printf("     CHEAT ENGINE POINTER ADAPTER - MULTI-OS COMPILER ENGINE          \n");
    printf("     Mendukung Windows 95, Windows 98, WinXP, Win7, dan Win10        \n");
    printf("======================================================================\n");
}

int main(int argc, char* argv[]) {
    CompilerEnvironment env;
    print_banner();

    if (!compiler_detect_environment(&env)) {
        printf("[ERROR] Gagal mendeteksi environment compiler!\n");
        return 1;
    }

    printf("Compiler 32-bit (Win95/98/XP) : %s\n", env.mingw32_gcc);
    printf("Compiler 64-bit (Win7/Win10)  : %s\n", env.mingw64_gcc);

    if (argc > 1) {
        if (strcmp(argv[1], "--list") == 0 || strcmp(argv[1], "-l") == 0) {
            compiler_print_targets();
            return 0;
        }

        /* Check if specific OS target was requested */
        int target_id = -1;
        if (strstr(argv[1], "95")) target_id = TARGET_WIN95;
        else if (strstr(argv[1], "98")) target_id = TARGET_WIN98;
        else if (strstr(argv[1], "xp") || strstr(argv[1], "XP")) target_id = TARGET_WINXP;
        else if (strstr(argv[1], "7")) target_id = TARGET_WIN7;
        else if (strstr(argv[1], "10")) target_id = TARGET_WIN10;

        if (target_id >= 0) {
            int ok = compiler_build_target(&env, (OSTargetID)target_id);
            return ok ? 0 : 1;
        }

        printf("\nArgumen tidak dikenali: %s\n", argv[1]);
        printf("Gunakan:\n");
        printf("  builder.exe           -> Bangun semua target (Win95, 98, XP, 7, 10)\n");
        printf("  builder.exe win95     -> Bangun hanya pointer_adapter_win95.exe\n");
        printf("  builder.exe win98     -> Bangun hanya pointer_adapter_win98.exe\n");
        printf("  builder.exe winxp     -> Bangun hanya pointer_adapter_winxp.exe\n");
        printf("  builder.exe win7      -> Bangun hanya pointer_adapter_win7.exe\n");
        printf("  builder.exe win10     -> Bangun hanya pointer_adapter_win10.exe\n");
        printf("  builder.exe --list    -> Tampilkan detail konfigurasi setiap OS\n");
        return 1;
    }

    /* Build all targets by default */
    compiler_print_targets();
    int ok = compiler_build_all(&env);
    return ok ? 0 : 1;
}
