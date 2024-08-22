#include "binread.h"
#include <cstdio>
#include <stdio.h>

Binf* Bin_readf(const std::string& file_path) {
    FILE* file = fopen(file_path.c_str(), "rb");
    if (file == nullptr) {
        printf("FILE NOT FOUND: %s\n", file_path);
        return nullptr;
    }

    fseek(file, 0, SEEK_END);
    unsigned int size = ftell(file);
    fseek(file, 0, SEEK_SET);

    float* buffer = new float[size];
    fread(buffer, sizeof(float), size, file);

    fclose(file);

    Binf* bin = new Binf();
    bin->buffer = buffer;
    bin->size = size;
    return bin;
}

Bini* Bin_readi(const std::string& file_path) {
    FILE* file = fopen(file_path.c_str(), "rb");
    if (file == nullptr) {
        printf("FILE NOT FOUND: %s\n", file_path);
        return nullptr;
    }

    fseek(file, 0, SEEK_END);
    unsigned int size = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned int* buffer = new unsigned int[size];
    fread(buffer, sizeof(int), size, file);

    fclose(file);

    Bini* bin = new Bini();
    bin->buffer = buffer;
    bin->size = size;
    return bin;
}