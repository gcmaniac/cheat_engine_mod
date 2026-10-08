# 🛠️ Panduan Pengembang (Developer Guide)

---

## 🏛️ Arsitektur Proyek

Kode sumber proyek dibagi secara modular untuk memisahkan logika pemrosesan data, antarmuka grafis, lokalisasi bahasa, dan sistem kompilasi target OS:

```
cheat_engine_mod/
├── src/
│   ├── ct_processor.h / .c   # Parsing XML .CT, aritmatika pointer, file I/O
│   ├── gui.h / .c            # Antarmuka Win32 GUI murni (Win95 s/d Win10/11)
│   ├── lang.h / .c           # Sistem lokalisasi multi-bahasa (ID & EN)
│   ├── resource.rc           # Definisi resource icon aplikasi Windows
│   └── main.c                # Titik masuk WinMain & pemrosesan argumen CLI
│
├── builder/
│   ├── compiler_config.h     # Konfigurasi flags arsitektur CPU & PE Subsystem per OS
│   ├── compiler_manager.h/.c # Mesin deteksi toolchain & orkestrasi build
│   └── builder_main.c        # Program CLI builder (builder.exe)
│
├── lang/
│   ├── id.json               # File referensi bahasa Indonesia
│   └── en.json               # File referensi bahasa Inggris
│
├── assets/
│   ├── app.ico               # Multi-resolution icon (16x16 s/d 256x256)
│   ├── logo.jpg              # Logo resmi aplikasi
│   └── banner.jpg            # Banner visual game hacking
│
├── docs/                     # Dokumentasi pengguna, arsitektur, dan panduan
└── build.bat                 # Script batch otomatis 1-klik
```

---

## ⚙️ Prinsip Kompatibilitas Multi-OS (Win95 hingga Win10)

Membangun program tunggal yang dapat berjalan di sistem operasi vintage (Windows 95/98) hingga sistem operasi modern (Windows 10/11) memerlukan perhatian khusus pada PE Subsystem, Win32 API, dan CRT Linkage:

### 1. Pure ANSI Win32 API
- Windows 95 dan Windows 98 **tidak memiliki** implementasi Unicode (`...W`) secara default (hanya ANSI `...A`).
- Oleh karena itu, semua panggilan API menggunakan varian ANSI: `CreateWindowExA`, `GetOpenFileNameA`, `SetWindowTextA`, `RegisterClassExA`.
- Ukuran struktur `OPENFILENAMEA` dibatasi ke `OPENFILENAME_SIZE_VERSION_400` (76 byte) agar tidak memicu error `CDERR_STRUCTSIZE` di Windows 95/98/NT4.

### 2. Static CRT Linkage
- Binari dikompilasi dengan `-static -static-libgcc`.
- Bergantung murni pada DLL sistem standar yang selalu ada di semua instalasi Windows:
  `KERNEL32.DLL`, `USER32.DLL`, `GDI32.DLL`, `COMDLG32.DLL`, `SHELL32.DLL`, dan `MSVCRT.DLL`.
- Menghindari ketergantungan pada runtime UCRT modern (`api-ms-win-crt-*`) yang tidak ada di Windows 95/98/XP.

### 3. Matriks PE Subsystem per Target

| Target OS | Arsitektur | Flag Subsystem | Flag Versi OS | CPU Set |
|---|---|---|---|---|
| **Win 95** | 32-bit x86 | `-Wl,--subsystem,windows:4.0` | `-Wl,--major-os-version,4 -Wl,--minor-os-version,0` | `-march=i486 -mtune=generic` |
| **Win 98** | 32-bit x86 | `-Wl,--subsystem,windows:4.10`| `-Wl,--major-os-version,4 -Wl,--minor-os-version,10`| `-march=i586 -mtune=pentium2` |
| **Win XP** | 32-bit x86 | `-Wl,--subsystem,windows:5.1` | `-Wl,--major-os-version,5 -Wl,--minor-os-version,1` | `-march=i686 -mtune=pentium4 -msse2` |
| **Win 7**  | 64-bit x64 | `-Wl,--subsystem,windows:6.1` | `-Wl,--major-os-version,6 -Wl,--minor-os-version,1` | `-march=core2 -mtune=generic` |
| **Win 10** | 64-bit x64 | `-Wl,--subsystem,windows:6.0` | `-Wl,--major-os-version,6 -Wl,--minor-os-version,0` | `-march=x86-64 -mtune=generic` |

> [!NOTE]
> Nilai `MajorSubsystemVersion` pada Windows PE loader memiliki batas maksimum arsitektur **6.0** (NT 6.x mencakup Vista, 7, 8, 10, dan 11). Menyetel subsystem ke `10.0` akan memicu error `0xc000007b` (*STATUS_INVALID_IMAGE_FORMAT*) oleh kernel loader.

---

## 🌐 Menambahkan Bahasa Baru

Untuk menambahkan bahasa baru (misal: Jepang `"ja"` atau Spanyol `"es"`):
1. Buka [`src/lang.c`](file:///d:/c_projects/cheat_engine_mod/src/lang.c).
2. Buat array kamus baru `G_LANG_XX[]` dengan kunci yang sama.
3. Tambahkan opsi di fungsi `lang_set()` dan dropdown combobox di [`src/gui.c`](file:///d:/c_projects/cheat_engine_mod/src/gui.c).
4. Buat file referensi `lang/xx.json` di folder `lang/`.
