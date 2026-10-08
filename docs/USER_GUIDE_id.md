# 📖 Panduan Pengguna (User Guide) - Cheat Engine Pointer Adapter

---

## 📌 Pendahuluan

**Cheat Engine Pointer Adapter** adalah utilitas ringan yang dirancang khusus untuk memodifikasi alamat pointer di dalam file tabel Cheat Engine (`.CT`) secara massal (*bulk edit*). 

Alat ini sangat berguna ketika Anda bermain game (terutama melalui emulator seperti PCSX2, RPCS3, Dolphin, dll.) dan alamat basis memori emulator berpindah (*relocated* / ASLR) setelah emulator diperbarui atau dijalankan ulang.

---

## 🖥️ Antarmuka Program

Antarmuka dirancang dengan **Native Win32 GUI murni** yang sangat ringan, tanpa dependensi framework berat, sehingga langsung berjalan secepat kilat di Windows 95, 98, XP, 7, 8, hingga Windows 10 & 11.

Elemen antarmuka utama:
1. **Pemilihan Bahasa (Language Selector)**: Terletak di pojok kanan atas (`ID - Indonesia` / `EN - English`). Teks antarmuka akan berubah seketika tanpa perlu restart.
2. **Input File Cheat Engine (.CT)**: Kotak teks path file dilengkapi tombol **Browse...** dan dukungan **Drag & Drop** langsung dari Windows Explorer.
3. **Pointer Original**: Alamat pointer acuan yang saat ini tersimpan di dalam file tabel `.CT`.
4. **Pointer Pengganti**: Alamat pointer acuan baru yang Anda dapatkan dari hasil scan di sesi game yang sedang aktif.
5. **Tombol "Hitung Selisih & Refaktor Pointer"**: Memulai kalkulasi dan pemrosesan otomatis.
6. **Kotak Log & Status**: Menampilkan riwayat pemrosesan, nilai selisih (*delta*), jumlah pointer yang diubah, dan lokasi penyimpanan file baru.

---

## 🚀 Langkah-langkah Penggunaan

### Langkah 1: Pilih File Tabel Cheat Engine (`.CT`)
- Klik tombol **Browse...** untuk memilih file `.CT` yang ingin Anda perbarui, ATAU
- Cukup seret (*drag and drop*) file `.CT` dari folder Anda langsung ke jendela program.

### Langkah 2: Tentukan Pointer Acuan (Original vs Pengganti)
Cari satu cheat yang mudah ditemukan pointernya di tabel lama (misalnya: *Item Qty* atau *Gold*):
1. **Pointer Original**:
   Lihat alamat pointer cheat tersebut di dalam tabel `.CT` lama Anda.  
   *Contoh:* `"pcsx2-qt.exe"+0317C7E8`
2. **Pointer Pengganti**:
   Lakukan scan di Cheat Engine pada game Anda yang sedang berjalan untuk mencari alamat baru dari cheat yang sama.  
   *Contoh hasil scan baru:* `"pcsx2-qt.exe"+031FD968`

### Langkah 3: Klik Tombol Refaktor
Klik tombol **"Hitung Selisih & Refaktor Pointer"** (*Calculate Delta & Refactor Pointers*).

Program secara otomatis akan:
1. Menghitung selisih matematis bertanda:  
   $$\text{Delta} = \text{Pengganti} - \text{Original} = 031FD968 - 0317C7E8 = +81180$$
2. Memindai seluruh tag `<Address>` di file `.CT`.
3. Menyesuaikan semua pointer dengan modul yang sama (`pcsx2-qt.exe`) dengan menambahkan/mengurangkan selisih tersebut.
4. Membiarkan pointer dengan modul lain (seperti modul internal game `FFX.exe`) tetap aman tanpa tersentuh.
5. Menyimpan file hasil modifikasi di folder yang sama dengan penamaan aman otomatis (misal: `NamaTabel_modified.CT` atau dengan nomor urut `(1)`, `(2)` jika sudah ada).

---

## 💡 Tips & Trik

- **Placeholder Cerdas**: Input pointer dilengkapi dengan contoh abu-abu (*grayed placeholder*). Begitu Anda mengklik kotak tersebut, teks contoh otomatis terhapus sehingga Anda bisa langsung *paste* (*Ctrl+V*) tanpa repot menghapus manual.
- **Kasus Pengurangan (Delta Negatif)**: Jika alamat pengganti lebih kecil dari original, program secara otomatis menghitung delta negatif dan mengurangkan semua alamat tanpa merusak format heksadesimal 8 digit (`%08X`).
