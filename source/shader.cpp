#include "shader.h"
#include "error_printer.h"

#include <fstream>
#include <sstream>

Shader::Shader(const char* vertex_file_path, const char* fragment_file_path)
{
    std::string vertex_code;
    std::string fragment_code;
    
    std::ifstream vertex_shader_file;
    std::ifstream fragment_shader_file;

    vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragment_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vertex_shader_file.open(vertex_file_path);
        fragment_shader_file.open(fragment_file_path);

        std::stringstream vertex_shader_string_stream;
        std::stringstream fragment_shader_string_stream;

        vertex_shader_string_stream << vertex_shader_file.rdbuf();
        fragment_shader_string_stream << fragment_shader_file.rdbuf();

        vertex_shader_file.close();
        fragment_shader_file.close();

        vertex_code = vertex_shader_string_stream.str();
        fragment_code = fragment_shader_string_stream.str();
    }
    catch(const std::exception& e)
    {
        ErrorPrinter::PrintError(e.what());
    }

    const char* vertex_shader_code = vertex_code.c_str();
    const char* fragment_shader_code = fragment_code.c_str();

    
    
}

void Shader::Use()
{

}

void Shader::SetBool(const std::string& name, bool value) const
{

}

void Shader::SetInt(const std::string& name, int value) const
{

}

void Shader::SetFloat(const std::string& name, float value) const
{

}