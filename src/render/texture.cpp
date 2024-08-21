#include "texture.h"
#include "../vendor/stb_image.h"

Texture* Texture_new(const std::string& file_path) {
    Texture* texture = new Texture();
    texture->id = 0;
    texture->file_path = file_path;
    texture->local_buffer = nullptr;

    stbi_set_flip_vertically_on_load(1);
    texture->local_buffer = stbi_load(file_path.c_str(), &texture->w, &texture->h, &texture->bpp, 4);

    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture->w, texture->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->local_buffer);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (texture->local_buffer) {
        stbi_image_free(texture->local_buffer);
    }

    return texture;
}

void Texture_bind(Texture* texture, unsigned int slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture->id);
}

void Texture_unbind(Texture* texture) {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture_delete(Texture* texture) {
    glDeleteTextures(1, &texture->id);
    delete texture;
}