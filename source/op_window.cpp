#include "op_window.h"
#include "error_printer.h"

OpWindow::OpWindow(const glm::vec3& background_color)
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
    glfwSetFramebufferSizeCallback(window, on_window_size_changed);

    glClearColor(background_color.x, background_color.y, background_color.z, 1.0f);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, on_mouse_moved);
}