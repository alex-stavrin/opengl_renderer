#include "shader.h"
#include "error_printer.h"
#include <glm/gtc/type_ptr.hpp>

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

    int success;
    char info_log[512];

    unsigned vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_shader_code, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, info_log);
        ErrorPrinter::PrintError(info_log);
    }

    unsigned fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_shader_code, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, info_log);
        ErrorPrinter::PrintError(info_log);
    }
    
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);

    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(id, 512, NULL, info_log);
        ErrorPrinter::PrintError(info_log);
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::Use()
{
    glUseProgram(id);
}

void Shader::SetBool(const std::string& name, bool value) const
{
    GLint uniform_location = glGetUniformLocation(id, name.c_str());
    glUniform1i(uniform_location, (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
    GLint uniform_location = glGetUniformLocation(id, name.c_str());
    glUniform1i(uniform_location, value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
    GLint uniform_location = glGetUniformLocation(id, name.c_str());
    glUniform1f(uniform_location, value);
}

void Shader::SetMatrix(const std::string& name, const glm::mat4& value)
{
    GLint uniform_location = glGetUniformLocation(id, name.c_str());
    glUniformMatrix4fv(uniform_location, 1, GL_FALSE, glm::value_ptr(value));
}