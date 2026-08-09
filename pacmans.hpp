#pragma once

#include <vector>

#include "package.hpp"

class backend
{
public:
    backend() = default;

    virtual void installPkgs(const std::vector<package> &pkgs) = 0;
    virtual std::vector<package> savePkgs() = 0;

    virtual ~backend() = default;
};

class apt : public backend
{
public:
    apt() = default;

    void installPkgs(const std::vector<package> &pkgs) override;
    std::vector<package> savePkgs() override;
};

class dnf : public backend
{
public:
    dnf() = default;

    void installPkgs(const std::vector<package> &pkgs) override;
    std::vector<package> savePkgs() override;
};


class pacman : public backend
{
public:
    pacman() = default;

    void installPkgs(const std::vector<package> &pkgs) override;
    std::vector<package> savePkgs() override;
};


class zypper : public backend
{
public:
    zypper() = default;

    void installPkgs(const std::vector<package> &pkgs) override;
    std::vector<package> savePkgs() override;
};

class xbps : public backend
{
public:
    xbps() = default;

    void installPkgs(const std::vector<package> &pkgs) override;
    std::vector<package> savePkgs() override;
};
