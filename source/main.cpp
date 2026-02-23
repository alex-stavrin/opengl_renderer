#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "error_printer.h"

void on_window_size_changed(GLFWwindow* window, int new_width, int new_height)
{
    glViewport(0, 0, new_width, new_height);
}

int main()
{
    glfwInit();

    // Set OpenGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Make window and assign to it our context
    GLFWwindow* window = glfwCreateWindow(800, 600, "opengl_renderer", NULL, NULL);
    if(window == nullptr)
    {
        ErrorPrinter::PrintError("Failed to create GLFW window.");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Init GLAD
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        ErrorPrinter::PrintError("Failed to initialize GLAD");
    }

    // Set viewport properties
    glViewport(0,0,800,600);

    // On window size changed. Change also the viewport
    glfwSetFramebufferSizeCallback(window, on_window_size_changed);

    // Render loop
    while(!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}