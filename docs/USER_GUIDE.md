# 📖 User Guide - Cheat Engine Pointer Adapter

---

## 📌 Overview

**Cheat Engine Pointer Adapter** is a specialized, featherlight utility designed to perform fast bulk editing of pointer addresses inside Cheat Engine Table files (`.CT`).

It is especially valuable when playing games via emulators (such as PCSX2, RPCS3, Dolphin, etc.) or PC titles where base memory addresses relocate across runs or emulator version updates due to ASLR (*Address Space Layout Randomization*).

---

## 🖥️ Graphical User Interface

The application interface is crafted using **pure Native Win32 GUI**, requiring no bulky runtimes or modern framework dependencies. As a result, it starts instantaneously and runs smoothly on Windows 95, 98, XP, 7, 8, 10, and 11.

Key UI components:
1. **Language Selector**: Located at the top right corner (`EN - English` / `ID - Indonesia`). Switching languages updates all UI text, labels, and dialogs dynamically without restarting.
2. **Cheat Engine File (.CT) Path**: Edit box with an adjacent **Browse...** button, plus full **Drag & Drop** support.
3. **Original Pointer**: The base reference pointer currently stored inside your `.CT` table file.
4. **Replacement Pointer**: The newly identified working pointer discovered via scanning in your active game session.
5. **"Calculate Delta & Refactor Pointers" Button**: Executes the signed difference calculation and modifies all matching addresses in the file.
6. **Activity Log & Status Area**: Displays real-time progress, calculated delta in hexadecimal and decimal, count of scanned vs updated pointers, and the path where the new `.CT` file was saved.

---

## 🚀 Step-by-Step Instructions

### Step 1: Select Your Cheat Table File (`.CT`)
- Click **Browse...** to pick the `.CT` file you wish to update, OR
- Simply **Drag & Drop** the `.CT` file from Windows Explorer into the application window.

### Step 2: Identify Reference Pointers (Original vs Replacement)
Locate any single cheat entry in your table that is straightforward to re-scan in your game (for example, *Item Quantity* or *Player Gold*):
1. **Original Pointer**:
   Inspect the address of that cheat in your existing table.  
   *Example:* `"pcsx2-qt.exe"+0317C7E8`
2. **Replacement Pointer**:
   Perform a memory scan or pointer scan in Cheat Engine on your active game session to locate the new valid address for the exact same cheat.  
   *Example:* `"pcsx2-qt.exe"+031FD968`

### Step 3: Click Refactor
Click the **"Calculate Delta & Refactor Pointers"** button.

The application automatically:
1. Calculates the signed difference:  
   $$\text{Delta} = \text{Replacement} - \text{Original} = 031FD968 - 0317C7E8 = +81180$$
2. Scans every `<Address>` tag across the `.CT` file.
3. Adjusts all pointers matching the target module (`pcsx2-qt.exe`) by adding/subtracting the delta.
4. Protects non-matching module pointers (such as internal game addresses like `"FFX.exe"`) from being altered.
5. Writes the result to a new file in the root folder using non-destructive incremented naming (e.g. `TableName_modified.CT` or `(1)`, `(2)` if already existing).

---

## 💡 Practical Tips

- **Smart Placeholders**: The pointer text boxes feature grayed example values. Clicking inside either input automatically clears the sample text and places the cursor at the beginning, allowing you to paste (`Ctrl+V`) immediately.
- **Negative Shifts**: If the replacement address is lower than the original, the tool handles negative offsets gracefully while preserving 8-digit zero-padded hexadecimal formatting (`%08X`).
