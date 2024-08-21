#include "shader.h"
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Shader* Shader_new(const std::string& vs_file_path, const std::string& fs_file_path) {
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
    Shader* shader = Shader_new(vs_file_path, fs_file_path);
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

void Shader_bind(Shader* shader) {
    glUseProgram(shader->program_id);
}

void Shader_unbind(Shader* shader) {
    glUseProgram(0);
}

void Shader_set_uniform1i(Shader* shader, const std::string& name, int v0) {
    glUniform1i(Shader_get_uniform_location(shader, name.c_str()), v0);
}

void Shader_set_uniform1f(Shader* shader, const std::string& name, float v0) {
    glUniform1f(Shader_get_uniform_location(shader, name.c_str()), v0);
}

void Shader_set_uniform2f(Shader* shader, const std::string& name, float v0, float v1) {
    glUniform2f(Shader_get_uniform_location(shader, name.c_str()), v0, v1);
}

void Shader_set_uniform3f(Shader* shader, const std::string& name, float v0, float v1, float v2) {
    glUniform3f(Shader_get_uniform_location(shader, name.c_str()), v0, v1, v2);
}

void Shader_set_uniform4f(Shader* shader, const std::string& name, float v0, float v1, float v2, float v3) {
    glUniform4f(Shader_get_uniform_location(shader, name.c_str()), v0, v1, v2, v3);
}

void Shader_set_uniform_mat4f(Shader* shader, const std::string& name, glm::mat4 matrix) {
    glUniformMatrix4fv(Shader_get_uniform_location(shader, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
}

int Shader_get_uniform_location(Shader* shader, const std::string& name) {
    if (shader->uniform_cache.find(name) != shader->uniform_cache.end())
        return shader->uniform_cache[name];

    auto loc = glGetUniformLocation(shader->program_id, name.c_str());
    if (loc == -1)
        printf("Warning: uniform %s doesn't exist\n", name.c_str());
    else
        shader->uniform_cache[name] = loc;

    return loc;
}

void Shader_delete(Shader* shader) {
    glDeleteProgram(shader->program_id);
    delete shader;
}