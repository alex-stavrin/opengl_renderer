#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

// Class abstraction for handling windows
// Right now we assume we will always have one window

class OpWindow
{
    private:
        GLFWwindow* glfw_window;

    public:
        OpWindow(const glm::vec3& background_color);
        GLFWwindow* GetWindowGLFW() {return glfw_window;};
};