#pragma once
#include <string>
#include <unordered_map>


struct Shader {
    unsigned int id;
    unsigned int program_id;
    std::string vs_file_path;
    std::string vs_source;
    std::string fs_file_path;
    std::string fs_source;
    std::unordered_map<std::string, int> uniform_cache;
};

Shader* Shader_new(const std::string& vs_file_path, const std::string& fs_file_path);
unsigned int Shader_compile(const std::string& source, unsigned int type);
unsigned int Shader_finalize(Shader* shader);
void Shader_bind(Shader* shader);
void Shader_unbind(Shader* shader);
void Shader_set_uniform4f(Shader* shader, const std::string& name, float v0, float v1, float v2, float v3);
int Shader_get_uniform_location(Shader* shader, const std::string& name);
Shader* Shader_gimme(const std::string& vs_file_path, const std::string& fs_file_path);
void Shader_delete(Shader* shader);
