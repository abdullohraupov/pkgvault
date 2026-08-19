Name:           pkgvault
Version:        1.0
Release:        1%{?dist}
Summary:        A minimalist CLI tool to save and restore installed packages

License:        GPL-3.0-or-later
URL:            https://github.com/abdullohraupov/pkgvault
Source0:        %{url}/archive/refs/heads/master.tar.gz

BuildRequires:  cmake
BuildRequires:  gcc-c++
BuildRequires:  make
BuildRequires:  nlohmann_json-devel

%description
PkgVault is a command-line utility for saving and loading package lists across different Linux distributions (apt, dnf, pacman, zypper, xbps).

%prep
%autosetup -n %{name}-master

%build
%cmake
%cmake_build

%install
%cmake_install

%files
%license LICENSE
%doc README.md
%{_bindir}/pkgvault

%changelog
* Wed Aug 19 2026 Abdulloh Raupov <abdullohraupov.work@gmail.com> - 1.1
- Initial build from main branch

* Sun Aug 09 2026 Abdulloh Raupov <abdullohraupov.work@gmail.com> - 1.0
- Initial release
