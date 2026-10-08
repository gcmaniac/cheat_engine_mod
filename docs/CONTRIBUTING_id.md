# 🤝 Panduan Kontribusi (Contributing Guide)

Terima kasih atas minat Anda untuk berkontribusi pada pengembangan **Cheat Engine Pointer Adapter**!

Proyek ini adalah perangkat lunak sumber terbuka (*open source*) yang dibuat untuk membantu komunitas gamer dan *reverse engineer* mengatasi keterbatasan Cheat Engine dalam menangani *bulk editing* alamat pointer.

---

## 🎯 Bidang Kontribusi yang Sangat Diharapkan

Kami mengundang kontributor dari seluruh dunia untuk berpartisipasi dalam hal berikut:

### 1. Penerjemahan / Lokalisasi (Translations)
Saat ini proyek mendukung **Bahasa Indonesia** dan **English**. Kami sangat menyambut kontribusi penambahan bahasa lain seperti:
- Bahasa Jepang (日本語)
- Bahasa Spanyol (Español)
- Bahasa Portugis (Português)
- Bahasa Rusia (Русский)
- Bahasa Mandarin (简体中文 / 繁體中文)

### 2. Fitur Baru
- **Lua Script Exporter**: Menghasilkan script Lua Cheat Engine untuk otomatisasi offset saat runtime.
- **Multi-Level Pointer Analyzer**: Mendukung penyesuaian offset bertingkat di tag `<Offsets><Offset>`.
- **Batch Processing**: Memproses beberapa file `.CT` sekaligus dalam satu direktori.

### 3. Pengujian Kompatibilitas Sistem Operasi
- Uji coba pada mesin fisik / virtual machine Windows 95 OSR2, Windows 98 SE, Windows ME, dan Windows XP SP3.
- Pelaporan *bug* atau ketidakcocokan visual di berbagai versi Windows.

---

## 🚀 Alur Pengajuan Kontribusi (Workflow)

1. **Fork** repositori ini di GitHub.
2. Buat branch fitur baru Anda:
   ```bash
   git checkout -b fitur/nama-fitur-baru
   ```
3. Lakukan perubahan kode dan pastikan semua target dapat dikompilasi tanpa *warning*:
   ```cmd
   build.bat
   ```
4. Commit perubahan Anda dengan pesan yang jelas:
   ```bash
   git commit -m "feat: tambahkan dukungan bahasa Jepang"
   ```
5. Push branch Anda dan buka **Pull Request (PR)** di GitHub.
