#pragma once
#include <string>


struct Shader {
    unsigned int id;
    unsigned int program_id;
    std::string vs_file_path;
    std::string vs_source;
    std::string fs_file_path;
    std::string fs_source;
};

Shader* Shader_prelude(const std::string& vs_file_path, const std::string& fs_file_path);
unsigned int Shader_compile(const std::string& source, unsigned int type);
unsigned int Shader_finalize(Shader* shader);
Shader* Shader_gimme(const std::string& vs_file_path, const std::string& fs_file_path);