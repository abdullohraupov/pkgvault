# Maintainer: Abdulloh Raupov <abdullohraupov.work@gmail.com>
pkgname=pkgvault
pkgver=1.0
pkgrel=1
pkgdesc="A minimalist CLI tool to save and restore installed packages across different Linux distributions."
arch=('x86_64' 'aarch64')
url="https://github.com/abdullohraupov/pkgvault"
license=('GPL3')
depends=('gcc-libs')
makedepends=('cmake' 'git' 'nlohmann-json')
source=("git+https://github.com/abdullohraupov/pkgvault.git#tag=v${pkgver}")
sha256sums=('SKIP')

build() {
  cmake -B build -S "$pkgname" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/usr
  cmake --build build
}

package() {
  DESTDIR="$pkgdir" cmake --install build
}
