#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

#include "error_printer.h"
#include "shader.h"

const unsigned starting_width = 800;
const unsigned starting_height = 600;

Shader* square_shader_ptr = nullptr;

glm::vec3 camera_position = glm::vec3(0,0,0);
glm::vec3 camera_front = glm::vec3(0,0,1);
glm::vec3 camera_up = glm::vec3(0,1,0);
glm::vec3 camera_right = glm::normalize(glm::cross(camera_front, camera_up));

float delta_time = 1;

void on_window_size_changed(GLFWwindow* window, int new_width, int new_height)
{
    if(square_shader_ptr)
    {
        glm::mat4 projection_matrix = glm::perspective(glm::radians(90.0f), (float)new_width / (float)new_height, 0.1f, 100.0f);
        square_shader_ptr->SetMatrix("projection_matrix", projection_matrix);
    }
    
    glViewport(0, 0, new_width, new_height);
}

void process_input(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    const float camera_speed = 2.5 * delta_time;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera_position += camera_front * camera_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera_position += -camera_front * camera_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera_position += camera_right * camera_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera_position += -camera_right * camera_speed;
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
    GLFWwindow* window = glfwCreateWindow(starting_width, starting_height, "opengl_renderer", NULL, NULL);
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
    glViewport(0,0,starting_width,starting_height);

    // On window size changed. Change also the viewport
    glfwSetFramebufferSizeCallback(window, on_window_size_changed);

    // Set Clear Color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    float cube_vertices[] = 
    {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    // specify how to draw the triangles for the square
    // unsigned indices[] = 
    // {
    //     0,1,3,
    //     1,2,3
    // };

    // Make and bind the vertex buffer
    unsigned vertex_buffer_object;
    glGenBuffers(1, &vertex_buffer_object);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    // Tell the vertex shader how to interpret the buffer
    unsigned vertex_array_object;
    glGenVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);

    GLsizei stride = 5 * sizeof(float);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    // texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Make and bind the element buffer
    // unsigned element_buffer_object;
    // glGenBuffers(1, &element_buffer_object);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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
        glActiveTexture(GL_TEXTURE0);
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

    data = stbi_load("./assets/awesomeface.png", &width, &height, &number_of_color_channels, 0);
    if(data)
    {
        unsigned texture;
        glGenTextures(1, &texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture);

        // texture wrapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
        // texture filtering (for magnification. texture > space)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
        // minmap filtering (for minification. texture < space)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        
        // pass image to texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
        // generate mip maps
        glGenerateMipmap(GL_TEXTURE_2D);   
    }
    else
    {
        ErrorPrinter::PrintError("Failed to load image");
    }
    stbi_image_free(data);

    Shader square_shader("./shaders/vertex/square.vs", "./shaders/fragment/square.fs");
    square_shader_ptr = &square_shader;
    square_shader.Use();

    square_shader.SetInt("texture0", 0);
    square_shader.SetInt("texture1", 1);

    // model matrix
    glm::mat4 model_matrix = glm::mat4(1.0f);
    model_matrix = glm::translate(model_matrix, glm::vec3(0.0,0.0,2.0));

    // view matrix


    // projection matrix
    glm::mat4 projection_matrix = glm::perspective(glm::radians(90.0f), (float)starting_width / (float)starting_height, 0.1f, 100.0f);
    
    square_shader.SetMatrix("model_matrix", model_matrix);
    square_shader.SetMatrix("projection_matrix", projection_matrix);

    glEnable(GL_DEPTH_TEST);
    
    float last_frame_time = 1;

    // Game loop
    while(!glfwWindowShouldClose(window))
    {
        float current_time = static_cast<float>(glfwGetTime());
        delta_time = current_time - last_frame_time;
        last_frame_time = current_time;

        // Input
        process_input(window);

        // Render
        glm::mat4 view_matrix = glm::lookAt(camera_position, camera_position + camera_front, camera_up);
        square_shader.SetMatrix("view_matrix", view_matrix);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}