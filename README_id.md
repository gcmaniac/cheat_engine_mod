<div align="center">

![Cyber Cheat Memory Hacking Interface](assets/banner.jpg)

# 🎯 Cheat Engine Pointer Adapter
### *Universal Multi-OS Bulk Pointer Refactor Utility for Cheat Engine Tables (.CT)*

<p align="center">
  <img src="assets/logo.jpg" alt="App Logo" width="130" style="border-radius: 20px;" />
</p>

[![Language: C](https://img.shields.io/badge/Language-C99%20%2F%20ANSI%20C-00599C?style=for-the-badge&logo=c)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Platform: Windows](https://img.shields.io/badge/Platform-Windows%2095%20s%2Fd%2011-0078D6?style=for-the-badge&logo=windows)](https://microsoft.com)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.style=for-the-badge)](LICENSE)
[![Architecture: Multi-OS PE](https://img.shields.io/badge/PE%20Subsystem-4.0%20to%206.0-orange?style=for-the-badge)](docs/DEVELOPER_GUIDE.md)
[![Multi-Language](https://img.shields.io/badge/Languages-ID%20%7C%20EN-purple?style=for-the-badge)](lang/)

🌐 **Bahasa:** [English](README.md) • **Bahasa Indonesia**

[**📖 Panduan Pengguna**](docs/USER_GUIDE_id.md) • [**🛠️ Panduan Pengembang**](docs/DEVELOPER_GUIDE_id.md) • [**🤝 Undangan Kontribusi**](docs/CONTRIBUTING_id.md)

---

</div>

## 📜 Latar Belakang Masalah (Background)

**Cheat Engine** adalah perangkat lunak *de-facto* terpopuler di dunia untuk menganalisis memori game dan membuat tabel cheat (`.CT`). Ribuan kreator membagikan tabel cheat yang sangat komprehensif berisi ratusan entri cheat untuk berbagai game retro dan modern.

Namun, terdapat satu **keterbatasan fatal** pada Cheat Engine:
> ⚠️ **Cheat Engine TIDAK memiliki kemampuan untuk melakukan Bulk Edit (pengubahan massal) pada alamat yang berupa pointer modul (contoh: `"pcsx2-qt.exe"+0317C7E8`).**

### Dampak Nyata pada Komunitas:
Saat bermain game melalui emulator (seperti **PCSX2**, **RPCS3**, **Dolphin**, **Citra**) atau game PC dengan proteksi ASLR (*Address Space Layout Randomization*), alamat basis memori emulator sering kali **berpindah** setiap kali emulator diperbarui atau dimulai ulang.

Jika sebuah tabel cheat memiliki **200+ entri cheat** (seperti tabel Final Fantasy X yang berisi ratusan pointer untuk *Stats Tidus, Yuna, Wakka, Inventory, Item Qty, Equipment Slot, Blitzball, dll.*):
1. **Semua cheat tersebut langsung rusak / tidak berfungsi** saat memori emulator bergeser.
2. Di Cheat Engine, pengguna terpaksa harus **membuka satu per satu entri cheat, menghitung selisih alamat secara manual dengan kalkulator heksadesimal, lalu mengetik ulang alamatnya ratusan kali**.
3. Proses manual ini memakan waktu **berjam-jam**, membosankan, dan sangat rentan terhadap kesalahan ketik (*human error*).

### 💡 Solusi: Cheat Engine Pointer Adapter
**Cheat Engine Pointer Adapter** diciptakan untuk memangkas proses berjam-jam tersebut menjadi **kurang dari 1 detik**:
1. Anda cukup mencari **satu pointer acuan baru** di sesi game aktif Anda (misalnya melalui *pointerscan* untuk *1st Item Qty*).
2. Masukkan alamat lama dan alamat baru ke dalam program ini.
3. Program secara otomatis menghitung selisih matematis (*delta*) bertanda dan **merefaktor seluruh ratusan pointer yang ada di file `.CT` secara instan**!

---

## ✨ Fitur Unggulan

- 🪟 **Kompatibilitas Multi-OS Ekstrem**: Berjalan mulus di **Windows 95, Windows 98, Windows XP, Windows 7, Windows 8, Windows 10, hingga Windows 11**.
- 🌐 **Dukungan Multi-Bahasa (Multi-Language)**: Beralih instan antara **Bahasa Indonesia** dan **English** langsung dari antarmuka program tanpa perlu restart.
- 🎨 **Antarmuka Grafis Native & Elegan**: Menggunakan Win32 GUI murni yang sangat cepat dan ringan (< 250 KB) dengan icon aplikasi resolusi tinggi tersemat di file PE.
- 🖱️ **Drag & Drop**: Seret dan jatuhkan file `.CT` dari Windows Explorer langsung ke jendela aplikasi.
- 💡 **Smart Grayed Placeholder**: Kotak input dilengkapi contoh format pointer berwarna abu-abu yang **otomatis terhapus dan fokus** saat diklik pengguna.
- 🛡️ **Pencocokan Modul Cerdas**: Hanya memodifikasi pointer dari modul target yang sesuai (misal: `"pcsx2-qt.exe"`), serta melindungi pointer modul internal game lain (seperti `"FFX.exe"`) agar tidak rusak.
- 🔢 **Aritmatika Hex Bertanda Presisi**: Menjaga format heksadesimal asli dengan bantalan nol (*zero-padding*, misal `%08X`) secara sempurna.
- 💾 **Penyimpanan Aman Otomatis**: Menyimpan hasil di folder root dengan penamaan otomatis bertingkat (`_modified.CT`, `(1)`, `(2)`) tanpa menimpa file asli Anda.

---

## 🚀 Panduan Penggunaan Cepat (Quick Start)

```
1. Jalankan salah satu executable (misal: pointer_adapter_win10.exe atau pointer_adapter_win95.exe).
2. Pilih file .CT Anda (klik "Browse..." atau Drag & Drop file ke jendela program).
3. Masukkan Pointer Original (alamat lama di tabel) dan Pointer Pengganti (alamat baru hasil scan).
4. Klik tombol "Hitung Selisih & Refaktor Pointer".
5. Selesai! File baru Anda siap dimuat di Cheat Engine.
```

*Panduan visual dan studi kasus lengkap tersedia di [docs/USER_GUIDE.md](docs/USER_GUIDE.md).*

---

## 📦 Hasil Kompilasi Executable

Setiap binary dikompilasi dengan konfigurasi subsystem dan arsitektur CPU yang disesuaikan secara presisi:

| File Executable | Target Sistem Operasi | Arsitektur | PE Subsystem | Keterangan |
|---|---|---|---|---|
| [`pointer_adapter_win95.exe`](pointer_adapter_win95.exe) | **Windows 95** | 32-bit x86 | `4.0` | Pure ANSI Win32, `i486`, static CRT, tanpa dependensi SSE |
| [`pointer_adapter_win98.exe`](pointer_adapter_win98.exe) | **Windows 98 / ME** | 32-bit x86 | `4.10` | Pure ANSI Win32, `pentium2`, static CRT |
| [`pointer_adapter_winxp.exe`](pointer_adapter_winxp.exe) | **Windows XP** | 32-bit x86 | `5.1` | Win32 Subsystem XP, `pentium4` / SSE2 |
| [`pointer_adapter_win7.exe`](pointer_adapter_win7.exe) | **Windows 7** | 64-bit x64 | `6.1` | 64-bit Core2 tune, static CRT |
| [`pointer_adapter_win10.exe`](pointer_adapter_win10.exe) | **Windows 10 / 11** | 64-bit x64 | `6.0` | 64-bit Modern x86-64, static CRT |

---

## 🛠️ Kompilasi dari Kode Sumber (Building from Source)

Proyek ini dilengkapi dengan **Compiler Engine terintegrasi** (`builder/`) dan script batch otomatis:

```cmd
# Kompilasi seluruh 5 target OS sekaligus (termasuk ikon & modul bahasa):
build.bat

# Kompilasi target tertentu menggunakan builder.exe:
builder.exe win10
builder.exe win7
builder.exe winxp
builder.exe win98
builder.exe win95

# Menampilkan informasi detail arsitektur tiap OS:
builder.exe --list
```

*Dokumentasi teknis compiler dan arsitektur PE tersedia di [docs/DEVELOPER_GUIDE.md](docs/DEVELOPER_GUIDE.md).*

---

## 🤝 Undangan Partisipasi & Kontribusi (Call for Contributors)

Proyek ini adalah inisiatif sumber terbuka (*open source*) yang didedikasikan untuk komunitas game modding internasional. Kami mengundang partisipasi Anda untuk:

1. 🌍 **Penerjemahan Bahasa Baru**: Tambahkan bahasa Anda (Jepang, Spanyol, Portugis, Jerman, dll.) di folder [`lang/`](lang/).
2. ⚡ **Pengembangan Fitur**:
   - Eksportir skrip Lua otomatis untuk Cheat Engine.
   - Analisis dan refaktor multi-level offset (`<Offsets><Offset>`).
   - Pemrosesan *batch* untuk memproses banyak file `.CT` sekaligus.
3. 🧪 **Pengujian Retro-Hardware**: Uji coba pada mesin fisik Windows 95, 98, atau XP lawas dan laporkan hasilnya.

Silakan baca panduan lengkap kontribusi di [**docs/CONTRIBUTING.md**](docs/CONTRIBUTING.md) atau langsung buka **Pull Request (PR)**!

---

## 📄 Lisensi

Proyek ini dilisensikan di bawah lisensi [MIT License](LICENSE). Bebas digunakan, dimodifikasi, dan didistribusikan untuk keperluan pribadi maupun komunitas.
