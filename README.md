# PkgVault 📦

![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg?style=flat-square&logo=cplusplus)
![Linux](https://img.shields.io/badge/Platform-Linux-FFD140.svg?style=flat-square&logo=linux&logoColor=black)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg?style=flat-square)](https://www.gnu.org/licenses/gpl-3.0)

**PkgVault** is a lightweight utility designed to back up and restore explicitly installed system packages across various Linux package managers.

Whether you're setting up a fresh system after a clean install or syncing software stacks across multiple machines running the same distribution, PkgVault provides a clean, reproducible JSON workflow for package management.

---

## Highlights ✨

- Multi-Distro Support: Compatible with `apt`, `dnf`, `pacman`, `zypper`, and `xbps` environments.
- Explicit Only: Filters out automatic dependencies (saves only packages you explicitly installed).
- Safe Execution: Built-in package name sanitization to prevent shell injection.
- Declarative Backups: Saves your system state into clean, human-readable JSON files.

---

## Supported Package Managers 📊

| Manager      | Distros                             |
|:-------------|:------------------------------------|
| **`pacman`** | Arch Linux, Manjaro, EndeavourOS    |
| **`dnf`**    | Fedora, RHEL, CentOS Stream, Nobara |
| **`apt`**    | Debian, Ubuntu, Pop!_OS, Linux Mint |
| **`zypper`** | openSUSE Leap / Tumbleweed          |
| **`xbps`**   | Void Linux                          |

---

## Installation 📦

### Fedora / RHEL / openSuse Leap (via Copr)

If you are running Fedora, RHEL, or OpenSuse Leap, you can install `pkgvault` directly from the official Copr repository:

```bash
# Enable the Copr repository
sudo dnf copr enable abdullohraupov/pkgvault

# Install PkgVault
sudo dnf install pkgvault
```

---

### Building from Source 🛠

#### Requirements

* C++17 compliant compiler (`gcc` >= 8 or `clang` >= 7)
* `cmake` (>= 3.14)
* `make` or `ninja`

#### Build Steps

```bash
# Clone the repository
git clone https://github.com/abdullohraupov/pkgvault.git
cd pkgvault

# Configure and build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)

# (Optional) Install system-wide
sudo cmake --install build
```

---

## Usage 📖

### Save Installed Packages

Export explicitly installed packages to a JSON file:

```bash
pkgvault -s my_packages.json
# or
pkgvault --save my_packages.json

```

### Restore Packages

Install packages listed in a JSON backup file:

```bash
pkgvault -i my_packages.json
# or
pkgvault --install my_packages.json
```

### Options Overview

```text
Usage: pkgvault [OPTIONS] <file.json>

Options:
  -s, --save <file>     Export current package list to JSON
  -i, --install <file>  Install packages from JSON file
  -v, --version         Display application version
  -h, --help            Display this help message
```

---

## Backup Format Example 📄

The generated `.json` backup file example:

```json
{
  "packages": [
    "cmake",
    "gcc-c++",
    "git",
    "neovim",
    "zsh"
  ]
}
```

---

## License 📄

Distributed under the **GNU General Public License v3.0 (GPLv3)**. See [`LICENSE`](LICENSE) for details.
