#pragma once

#include <string>

class Core
{
public:
    constexpr Core() = default;
    ~Core() = default;

    void start(int argc, char *argv[]) const;

private:
    void query(const std::string &input) const;
};
