#include "shader.h"
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Shader* Shader_prelude(const std::string& vs_file_path, const std::string& fs_file_path) {
    Shader* shader = new Shader();
    shader->vs_file_path = vs_file_path;
    shader->fs_file_path = fs_file_path;
    std::getline(std::ifstream(vs_file_path), shader->vs_source, '\0');
    std::getline(std::ifstream(fs_file_path), shader->fs_source, '\0');

    printf("VERTEX\n%s\n", shader->vs_source.c_str());
    printf("=========================\n");
    printf("FRAGMENT\n%s\n", shader->fs_source.c_str());

    return shader;
}

// The "Just give me a fucking shader" way to go about things.
// -----------------------------------------------------------
Shader* Shader_gimme(const std::string& vs_file_path, const std::string& fs_file_path) {
    Shader* shader = Shader_prelude(vs_file_path, fs_file_path);
    shader->program_id = Shader_finalize(shader);
    return shader;
}

unsigned int Shader_compile(const std::string& source, unsigned int type) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        if (type == GL_VERTEX_SHADER) {
            printf("[VERTEX] ");
        } else {
            printf("[FRAGMENT] ");
        }
        printf("Failed to compile shader: %s\n", message);
    }
    return id;
}

unsigned int Shader_finalize(Shader* shader) {
    unsigned int program = glCreateProgram();
    unsigned int _vs = Shader_compile(shader->vs_source, GL_VERTEX_SHADER);
    unsigned int _fs = Shader_compile(shader->fs_source, GL_FRAGMENT_SHADER);

    glAttachShader(program, _vs);
    glAttachShader(program, _fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(_vs);
    glDeleteShader(_fs);

    return program;
}