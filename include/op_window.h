#pragma once

#include <vector>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include "camera.h"
#include "shader.h"

#define base_window_width 1280
#define base_window_height 720

// Class abstraction for handling windows
// Right now we assume we will always have one window

class OpWindow 
{
    private:
        GLFWwindow* glfw_window;
        Camera* camera;
        std::vector<Shader*> shaders;
        bool is_fullscreen;
        int last_window_width = base_window_width;
        int last_window_height = base_window_height;
        int last_window_x = 200;
        int last_window_y = 200;

        OpWindow() = default;
        ~OpWindow() = default;

    public:

        static OpWindow& GetInstance() 
        {
            static OpWindow instance;
            return instance;
        }

        OpWindow(const OpWindow&) = delete;
        OpWindow& operator=(const OpWindow&) = delete;

        static void Init(const glm::vec3& background_color);
        static GLFWwindow* GetWindowGLFW() {return GetInstance().glfw_window;};
        static void OnWindowSizeChanged(GLFWwindow* window, int new_width, int new_height);
        static void OnMouseMoved(GLFWwindow* window, double x_pos_in, double y_pos_in);
        static void SetCamera(Camera* n_camera) {GetInstance().camera = n_camera;};
        static void RegisterShader(Shader* shader);
        static void Tick();
        static void EndFrame();
        static void ToggleFullscreen();
        static std::pair<int, int> GetFrameBufferDimensions();
};