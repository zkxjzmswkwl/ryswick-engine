#pragma once
#include "renderer.h"
#include <string>

struct Texture {
    unsigned int id;
    std::string file_path;
    unsigned char* local_buffer;
    int w,h,bpp;
};

Texture* Texture_new(const std::string& file_path);
void Texture_bind(Texture* texture, unsigned int slot = 0);
void Texture_unbind(Texture* texture);
void Texture_delete(Texture* texture);