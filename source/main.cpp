#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "error_printer.h"
#include "shader.h"
#include "stb_image.h"

void on_window_size_changed(GLFWwindow* window, int new_width, int new_height)
{
    glViewport(0, 0, new_width, new_height);
}

void process_input(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
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

    // Set Clear Color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    float square_vertices[] = 
    {
        0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   1.0f, 1.0f,  // top right
        0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f,   0.0f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   0.0f, 1.0f   // top left 
    };

    // specify how to draw the triangles for the square
    unsigned indices[] = 
    {
        0,1,3,
        1,2,3
    };

    // Make and bind the vertex buffer
    unsigned vertex_buffer_object;
    glGenBuffers(1, &vertex_buffer_object);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square_vertices), square_vertices, GL_STATIC_DRAW);

    // Tell the vertex shader how to interpret the buffer
    unsigned vertex_array_object;
    glGenVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);

    GLsizei stride = 8 * sizeof(float);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Make and bind the element buffer
    unsigned element_buffer_object;
    glGenBuffers(1, &element_buffer_object);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    stbi_set_flip_vertically_on_load(true);  

    // Load image
    int width;
    int height;
    int number_of_color_channels;
    unsigned char* data = stbi_load("./assets/container.jpg", &width, &height, &number_of_color_channels, 0);
    if(data)
    {
        unsigned texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
    
        // texture wrapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
        // texture filtering (for magnification. texture > space)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
        // minmap filtering (for minification. texture < space)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
        // pass image to texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    
        // generate mip maps
        glGenerateMipmap(GL_TEXTURE_2D);    
    }
    else
    {
        ErrorPrinter::PrintError("Failed to load image");
    }
    stbi_image_free(data);


    Shader square_shader("./shaders/vertex/square.vs", "./shaders/fragment/square.fs");
    square_shader.Use();
    
    // Game loop
    while(!glfwWindowShouldClose(window))
    {
        // Input
        process_input(window);

        // Render
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}