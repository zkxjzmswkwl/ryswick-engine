#pragma once
#include <string>

struct Binf {
    void* buffer;
    unsigned int size;
};

struct Bini {
    void* buffer;
    unsigned int size;
};

Binf* Bin_readf(const std::string& file_path);
Bini* Bin_readi(const std::string& file_path);