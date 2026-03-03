#pragma once

#include "glad/glad.h"
#include <glm/glm.hpp>

#include <string>

class Shader
{
    public:
        unsigned id;

        Shader(const char* vertex_file_path, const char* fragment_file_path);
        void Use();
        void SetBool(const std::string& name, bool value) const;
        void SetInt(const std::string& name, int value) const;
        void SetFloat(const std::string& name, float value) const;
        void SetMatrix(const std::string& name, const glm::mat4& value);
        void SetVector3(const std::string& name, const glm::vec3& value);
};