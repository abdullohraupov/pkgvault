# 📦 PkgVault

![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg?style=flat-square&logo=cplusplus)
![Linux](https://img.shields.io/badge/Platform-Linux-FFD140.svg?style=flat-square&logo=linux&logoColor=black)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg?style=flat-square)](https://www.gnu.org/licenses/gpl-3.0)
![Build](https://img.shields.io/badge/build-passing-brightgreen.svg?style=flat-square)

**PkgVault** is a lightweight, zero-dependency C++17 CLI utility designed to back up and restore explicitly installed system packages across various Linux package managers.

Whether you're setting up a fresh system after a clean install or syncing software stacks across multiple machines running the same distribution, PkgVault provides a clean, reproducible JSON workflow for package management.

---

## ✨ Highlights

- 🚀 **Multi-Distro Support**: Compatible with `apt`, `dnf`, `pacman`, `zypper`, and `xbps` environments.
- 🔍 **Explicit Only**: Filters out automatic dependencies (saves only packages you explicitly installed).
- 🔐 **Safe Execution**: Built-in package name sanitization to prevent shell injection.
- 📂 **Declarative Backups**: Saves your system state into clean, human-readable JSON files.

---

## 📊 Supported Package Managers

| Manager      | Distros                          | Explicit Listing Method    |
|:-------------|:---------------------------------|:---------------------------|
| **`pacman`** | Arch Linux, Manjaro, EndeavourOS | `pacman -Qeq`              |
| **`dnf`**    | Fedora, RHEL, Nobara             | `rpm -qa --qf '%{NAME}\n'` |
| **`apt`**    | Debian, Ubuntu, Pop!_OS, Mint    | `apt-mark showmanual`      |
| **`zypper`** | openSUSE Leap / Tumbleweed       | `rpm -qa --qf '%{NAME}\n'` |
| **`xbps`**   | Void Linux                       | `xbps-query -m`            |

---

## 🛠 Building & Installation

### Requirements
- C++17 compatible compiler (`gcc` >= 8 or `clang` >= 7)
- `cmake` (>= 3.14)
- `make` or `ninja`

### Modern CMake Build

```bash
# Clone the repository
git clone [https://github.com/abdullohraupov/pkgvault.git](https://github.com/abdullohraupov/pkgvault.git)
cd pkgvault

# Configure and build
cmake -B build
cmake --build build -j$(nproc)

# (Optional) Install system-wide
sudo cmake --install build