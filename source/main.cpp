#include <iostream>
#include <vector>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "stb_image.h"

#include "error_printer.h"
#include "shader.h"
#include "camera.h"

// Window
int base_window_width = 1280;
int base_window_height = 720;

int last_window_width = base_window_width;
int last_window_height = base_window_height;
int last_window_x = 200;
int last_window_y = 200;

bool is_fullscreen = true;


Camera* camera_ptr = nullptr;
std::vector<Shader*> shaders;
float delta_time = 1;

void on_window_size_changed(GLFWwindow* window, int new_width, int new_height)
{
    for(Shader* shader : shaders)
    {
        if(shader)
        {
            glm::mat4 projection_matrix = glm::perspectiveLH(glm::radians(90.0f), (float)new_width / (float)new_height, 0.1f, 100.0f);
            shader->SetMatrix("projection_matrix", projection_matrix);
        }
    }
    
    glViewport(0, 0, new_width, new_height);
}

void toggle_fullscreen(GLFWwindow* window)
{
    if(is_fullscreen) // exit fullscreen (windowed)
    {
        glfwSetWindowMonitor(window, nullptr, last_window_x, last_window_y, last_window_width, last_window_height, 0);
    }
    else // fullscreen
    {
        glfwGetWindowPos(window, &last_window_x, &last_window_y);
        glfwGetWindowSize(window, &last_window_width, &last_window_height);
        
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }
    is_fullscreen = !is_fullscreen;
}

void process_input(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    // We have this thing to avoid switching many times in a row
    static bool f11_pressed = false;
    if(glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS)
    {
        if(!f11_pressed)
        {
            toggle_fullscreen(window);
            f11_pressed = true;
        }
    }
    else
    {
        f11_pressed = false;
    }
}

void on_mouse_moved(GLFWwindow* window, double x_pos_in, double y_pos_in)
{
    if(camera_ptr)
    {
        camera_ptr->OnMouseMoved(window, x_pos_in, y_pos_in);
    }
}

int main()
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
        return -1;
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

    // Set Clear Color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    float cube_vertices[] = {
        // Positions          // Normals           // UVs
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
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

    GLsizei stride = 8 * sizeof(float);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    // normal attribute 
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // uv attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

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
    
    Camera camera(window, glm::vec3(0.0f), 3, 0.05, true);
    camera_ptr = &camera;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, on_mouse_moved);

    glm::mat4 projection_matrix = glm::perspectiveLH(glm::radians(90.0f), (float)primary_monitor_width / (float)primary_monitor_height, 0.1f, 100.0f);

    glm::vec3 light_color = glm::vec3(1.0f,1.0f,1.0f);
    Shader light_shader("./shaders/light/light.vs", "./shaders/light/light.fs");
    shaders.push_back(&light_shader);
    light_shader.Use();

    glm::vec3 light_position = glm::vec3(2.0,2.0,4.0);
    glm::mat4 light_model_matrix = glm::mat4(1.0f);
    light_model_matrix = glm::translate(light_model_matrix, light_position);
    light_shader.SetMatrix("model_matrix", light_model_matrix);
    light_shader.SetMatrix("projection_matrix", projection_matrix);

    light_shader.SetVector3("light_color", light_color);

    Shader crate_shader("./shaders/crate/crate.vs", "./shaders/crate/crate.fs");
    shaders.push_back(&crate_shader);
    crate_shader.Use();

    crate_shader.SetVector3("light_color", light_color);
    crate_shader.SetVector3("light_position", light_position);

    crate_shader.SetInt("texture0", 0);
    crate_shader.SetInt("texture1", 1);

    // model matrix
    glm::mat4 crate_model_matrix = glm::mat4(1.0f);
    crate_model_matrix = glm::translate(crate_model_matrix, glm::vec3(0.0,0.5,2.0));
    
    crate_shader.SetMatrix("model_matrix", crate_model_matrix);
    crate_shader.SetMatrix("projection_matrix", projection_matrix);

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

        // Tick
        camera.Tick(delta_time);

        // Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        crate_shader.Use();
        crate_shader.SetMatrix("view_matrix", camera.GetViewMatrix());
        crate_shader.SetVector3("camera_position", camera.GetPosition());
        glDrawArrays(GL_TRIANGLES, 0, 36);

        light_shader.Use();
        light_shader.SetMatrix("view_matrix", camera.GetViewMatrix());
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}