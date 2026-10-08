# 🛠️ Developer Guide - Cheat Engine Pointer Adapter

---

## 🏛️ Project Architecture

The codebase is organized modularly to separate data parsing, graphical user interface, localization dictionaries, and target build orchestration:

```
cheat_engine_mod/
├── src/
│   ├── ct_processor.h / .c   # XML .CT parsing, pointer arithmetic, file I/O
│   ├── gui.h / .c            # Pure Win32 GUI implementation (Win95 to Win10/11)
│   ├── lang.h / .c           # Multi-language localization engine (EN & ID)
│   ├── resource.rc           # Windows PE application icon resource
│   └── main.c                # WinMain entry point & CLI parameter handling
│
├── builder/
│   ├── compiler_config.h     # Target definitions, CPU architecture & PE subsystem flags
│   ├── compiler_manager.h/.c # Toolchain detection engine & compiler execution
│   └── builder_main.c        # Standalone CLI compiler manager (builder.exe)
│
├── lang/
│   ├── en.json               # English translation dictionary
│   └── id.json               # Indonesian translation dictionary
│
├── assets/
│   ├── app.ico               # Multi-resolution icon (16x16 to 256x256)
│   ├── logo.jpg              # Official application emblem
│   └── banner.jpg            # Cyberpunk game hacking hero banner
│
├── docs/                     # User, developer, and contributing documentation
└── build.bat                 # One-click automated compilation script
```

---

## ⚙️ Multi-OS Compatibility Principles (Win95 to Win10/11)

Building a single C program that reliably runs on legacy operating systems (Windows 95, 98, ME, XP) while supporting modern 64-bit platforms (Windows 7, 8, 10, 11) demands careful control over PE subsystem versions, API choices, and runtime linking:

### 1. Pure ANSI Win32 APIs
- Windows 95 and Windows 98 **lack native Unicode (`...W`) implementations** by default (supporting only ANSI `...A` endpoints unless Microsoft Layer for Unicode / Unicows is present).
- All Win32 API invocations explicitly target ANSI functions: `CreateWindowExA`, `GetOpenFileNameA`, `SetWindowTextA`, `RegisterClassExA`.
- The `OPENFILENAMEA` structure size is capped at `OPENFILENAME_SIZE_VERSION_400` (76 bytes) to prevent `CDERR_STRUCTSIZE` errors on Windows 95/98/NT4.

### 2. Static CRT Linkage
- All target binaries are built with `-static -static-libgcc`.
- Binaries link exclusively against ubiquitous Windows system DLLs:
  `KERNEL32.DLL`, `USER32.DLL`, `GDI32.DLL`, `COMDLG32.DLL`, `SHELL32.DLL`, and `MSVCRT.DLL`.
- No dependencies on modern UCRT dynamic libraries (`api-ms-win-crt-*`) are introduced, guaranteeing out-of-the-box execution on unpatched legacy environments.

### 3. PE Subsystem Matrix per Target

| Target OS | Architecture | Subsystem Flag | OS Version Flag | CPU Instruction Tuning |
|---|---|---|---|---|
| **Win 95** | 32-bit x86 | `-Wl,--subsystem,windows:4.0` | `-Wl,--major-os-version,4 -Wl,--minor-os-version,0` | `-march=i486 -mtune=generic` |
| **Win 98** | 32-bit x86 | `-Wl,--subsystem,windows:4.10`| `-Wl,--major-os-version,4 -Wl,--minor-os-version,10`| `-march=i586 -mtune=pentium2` |
| **Win XP** | 32-bit x86 | `-Wl,--subsystem,windows:5.1` | `-Wl,--major-os-version,5 -Wl,--minor-os-version,1` | `-march=i686 -mtune=pentium4 -msse2` |
| **Win 7**  | 64-bit x64 | `-Wl,--subsystem,windows:6.1` | `-Wl,--major-os-version,6 -Wl,--minor-os-version,1` | `-march=core2 -mtune=generic` |
| **Win 10** | 64-bit x64 | `-Wl,--subsystem,windows:6.0` | `-Wl,--major-os-version,6 -Wl,--minor-os-version,0` | `-march=x86-64 -mtune=generic` |

> [!IMPORTANT]
> The `MajorSubsystemVersion` field inside the Windows PE image header has an architectural upper ceiling of **6.0** (NT 6.x accommodates Vista, 7, 8, 10, and 11). Setting the subsystem to `10.0` causes the kernel loader to reject the executable with `0xc000007b` (*STATUS_INVALID_IMAGE_FORMAT*).

---

## 🌐 Adding New Languages

To integrate a new language (for example, Japanese `"ja"` or Spanish `"es"`):
1. Open [`src/lang.c`](file:///d:/c_projects/cheat_engine_mod/src/lang.c).
2. Create a new dictionary array `G_LANG_XX[]` using the standard string keys.
3. Add the language code to `lang_set()` and append the choice to the ComboBox in [`src/gui.c`](file:///d:/c_projects/cheat_engine_mod/src/gui.c).
4. Add a matching JSON dictionary file in `lang/xx.json`.
