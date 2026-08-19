Name:           pkgvault
Version:        1.1
Release:        1%{?dist}
Summary:        A minimalist CLI tool to save and restore installed packages

License:        GPLv3
URL:            https://github.com/abdullohraupov/pkgvault
Source0:        %{url}/archive/v%{version}.tar.gz

BuildRequires:  cmake
BuildRequires:  gcc-c++
BuildRequires:  make
BuildRequires:  nlohmann_json-devel

%description
PkgVault is a command-line utility for saving and loading package lists across different Linux distributions (apt, dnf, pacman, zypper, xbps).

%prep
%autosetup

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
* Sun Aug 09 2026 Abdulloh Raupov <abdullohraupov.work@gmail.com> - 1.0
- Initial release
