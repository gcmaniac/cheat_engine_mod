#ifndef COMPILER_CONFIG_H
#define COMPILER_CONFIG_H

typedef enum {
    TARGET_WIN95 = 0,
    TARGET_WIN98,
    TARGET_WINXP,
    TARGET_WIN7,
    TARGET_WIN10,
    TARGET_COUNT
} OSTargetID;

typedef enum {
    TOOLCHAIN_MINGW32 = 0,
    TOOLCHAIN_MINGW64
} ToolchainType;

typedef struct {
    OSTargetID id;
    const char* os_name;
    const char* output_exe;
    ToolchainType toolchain;
    const char* arch_flags;
    const char* subsystem_flags;
    const char* os_version_flags;
    const char* desc;
} OSTargetDef;

static const OSTargetDef G_TARGETS[TARGET_COUNT] = {
    {
        TARGET_WIN95,
        "Windows 95",
        "pointer_adapter_win95.exe",
        TOOLCHAIN_MINGW32,
        "-march=i486 -mtune=generic",
        "-Wl,--subsystem,windows:4.0",
        "-Wl,--major-os-version,4 -Wl,--minor-os-version,0",
        "32-bit x86, Subsystem 4.0, pure ANSI Win32 API, no SSE, static CRT"
    },
    {
        TARGET_WIN98,
        "Windows 98",
        "pointer_adapter_win98.exe",
        TOOLCHAIN_MINGW32,
        "-march=i586 -mtune=pentium2",
        "-Wl,--subsystem,windows:4.10",
        "-Wl,--major-os-version,4 -Wl,--minor-os-version,10",
        "32-bit x86, Subsystem 4.10, Pentium 2 tune, static CRT"
    },
    {
        TARGET_WINXP,
        "Windows XP",
        "pointer_adapter_winxp.exe",
        TOOLCHAIN_MINGW32,
        "-march=i686 -mtune=pentium4 -msse2",
        "-Wl,--subsystem,windows:5.1",
        "-Wl,--major-os-version,5 -Wl,--minor-os-version,1",
        "32-bit x86, Subsystem 5.1, Pentium 4 / SSE2 tune, static CRT"
    },
    {
        TARGET_WIN7,
        "Windows 7",
        "pointer_adapter_win7.exe",
        TOOLCHAIN_MINGW64,
        "-march=core2 -mtune=generic",
        "-Wl,--subsystem,windows:6.1",
        "-Wl,--major-os-version,6 -Wl,--minor-os-version,1",
        "64-bit x86_64, Subsystem 6.1, Core2 tune, static CRT"
    },
    {
        TARGET_WIN10,
        "Windows 10",
        "pointer_adapter_win10.exe",
        TOOLCHAIN_MINGW64,
        "-march=x86-64 -mtune=generic",
        "-Wl,--subsystem,windows:6.0",
        "-Wl,--major-os-version,6 -Wl,--minor-os-version,0",
        "64-bit x86_64, Subsystem 6.0 (Win10/11 PE compatible), Modern x86-64, static CRT"
    }
};

#endif /* COMPILER_CONFIG_H */
