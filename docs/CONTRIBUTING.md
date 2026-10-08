# 🤝 Contributing Guide

Thank you for your interest in contributing to **Cheat Engine Pointer Adapter**!

This project is an open-source initiative designed to empower gamers, speedrunners, and reverse engineers by resolving Cheat Engine's inability to perform bulk pointer refactoring.

---

## 🎯 High-Value Areas for Contribution

We actively welcome community contributions in the following domains:

### 1. Translations & Localizations
The utility currently supports **English** and **Bahasa Indonesia**. We warmly encourage community pull requests adding other languages, including:
- Japanese (日本語)
- Spanish (Español)
- Portuguese (Português)
- Russian (Русский)
- Simplified / Traditional Chinese (简体中文 / 繁體中文)
- German (Deutsch)
- French (Français)

### 2. Feature Enhancements
- **Lua Script Exporter**: Generate a Cheat Engine Lua script that calculates and sets offsets dynamically at runtime.
- **Multi-Level Pointer Analyzer**: Provide automated adjustment for nested multi-level offsets inside `<Offsets><Offset>`.
- **Batch Directory Processing**: Allow users to process an entire folder of `.CT` tables at once.

### 3. Retro-Hardware & Environment Verification
- Testing on physical vintage PCs or virtualized machines running Windows 95 OSR2, Windows 98 SE, Windows ME, and Windows XP SP3.
- Submitting bug reports, visual anomalies, or performance benchmarks across diverse Windows revisions.

---

## 🚀 Contribution Workflow

1. **Fork** this repository on GitHub.
2. Create a dedicated feature branch:
   ```bash
   git checkout -b feature/your-feature-name
   ```
3. Implement your changes and verify that all target binaries compile cleanly without warnings:
   ```cmd
   build.bat
   ```
4. Commit your changes with concise, descriptive commit messages:
   ```bash
   git commit -m "feat: add Japanese translation support"
   ```
5. Push your feature branch and open a **Pull Request (PR)** on GitHub.
