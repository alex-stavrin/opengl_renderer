#include <utility>

#include "op_window.h"
#include "error_printer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

void OpWindow::Init(const glm::vec3& background_color)
{
    glfwInit();

    // Set OpenGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  
    
    // Get primary monitor
    GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();

    // Get info of primary monitor
    const GLFWvidmode* primary_monitor_video_mode = glfwGetVideoMode(primary_monitor);

    int primary_monitor_width = primary_monitor_video_mode->width;
    int primary_monitor_height = primary_monitor_video_mode->height;

    // Make window and assign to it our context
    GLFWwindow* window = glfwCreateWindow(primary_monitor_width, primary_monitor_height, "opengl_renderer", primary_monitor, NULL);
    if(window == nullptr)
    {
        ErrorPrinter::PrintError("Failed to create GLFW window.");
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    GetInstance().is_fullscreen = true;

    // VSync
    glfwSwapInterval(1);

    // Init GLAD
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        ErrorPrinter::PrintError("Failed to initialize GLAD");
    }

    // Set viewport properties
    glViewport(0, 0, primary_monitor_width, primary_monitor_height);

    // On window size changed. Change also the viewport
    glfwSetFramebufferSizeCallback(window, OnWindowSizeChanged);

    glClearColor(background_color.x, background_color.y, background_color.z, 1.0f);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, OnMouseMoved);

    GetInstance().glfw_window = window;
}

void OpWindow::OnWindowSizeChanged(GLFWwindow* window, int new_width, int new_height)
{
    for(Shader* shader : GetInstance().shaders)
    {
        if(shader)
        {
            glm::mat4 projection_matrix = glm::perspectiveLH(glm::radians(90.0f), (float)new_width / (float)new_height, 0.1f, 100.0f);
            shader->SetMatrix("projection_matrix", projection_matrix);
        }
    }
    
    glViewport(0, 0, new_width, new_height);
}

void OpWindow::OnMouseMoved(GLFWwindow* window, double x_pos_in, double y_pos_in)
{
    Camera* camera = GetInstance().camera;
    if(camera)
    {
        camera->OnMouseMoved(window, x_pos_in, y_pos_in);
    }
}

void OpWindow::RegisterShader(Shader* shader)
{
    GetInstance().shaders.push_back(shader);
}

void OpWindow::Tick()
{
    GLFWwindow* window = GetInstance().glfw_window;
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    static bool f11_pressed = false;
    if(glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS)
    {
        if(!f11_pressed)
        {
            ToggleFullscreen();
            f11_pressed = true;
        }
    }
    else
    {
        f11_pressed = false;
    }
}

void OpWindow::EndFrame()
{
    GLFWwindow* window = GetInstance().glfw_window;
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void OpWindow::ToggleFullscreen()
{
    GLFWwindow* window = GetInstance().glfw_window;
    if(GetInstance().is_fullscreen) // exit fullscreen (windowed)
    {
        glfwSetWindowMonitor(window, nullptr, GetInstance().last_window_x, GetInstance().last_window_y,
            GetInstance().last_window_width, GetInstance().last_window_height, 0);
    }
    else // fullscreen
    {
        glfwGetWindowPos(window, &(GetInstance().last_window_x), &(GetInstance().last_window_y));
        glfwGetWindowSize(window, &(GetInstance().last_window_width), &(GetInstance().last_window_height));
        
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }
    GetInstance().is_fullscreen = !(GetInstance().is_fullscreen);
}

std::pair<int, int> OpWindow::GetFrameBufferDimensions()
{
    int width, height;
    glfwGetFramebufferSize(GetInstance().glfw_window, &width, &height);
    return std::make_pair(width, height);
}