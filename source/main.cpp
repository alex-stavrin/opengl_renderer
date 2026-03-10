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
#include "op_window.h"
#include "ui.h"

float delta_time = 1;

int main()
{
    OpWindow::Init(glm::vec3(0.0f));

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

    stbi_set_flip_vertically_on_load(true);  
    // Load diffuse image
    int width;
    int height;
    int number_of_color_channels;
    unsigned char* data = stbi_load("./assets/container_diffuse.png", &width, &height, &number_of_color_channels, 0);
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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
        // generate mip maps
        glGenerateMipmap(GL_TEXTURE_2D);    
    }
    else
    {
        ErrorPrinter::PrintError("Failed to load image");
    }
    stbi_image_free(data);

    data = stbi_load("./assets/container_specular.png", &width, &height, &number_of_color_channels, 0);
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
    
    Camera camera(OpWindow::GetWindowGLFW(), glm::vec3(0.0f), 3.0, 0.05, true);
    OpWindow::SetCamera(&camera);

    auto [window_width, window_height] = OpWindow::GetFrameBufferDimensions();
    glm::mat4 projection_matrix = glm::perspectiveLH(glm::radians(90.0f), (float)window_width / (float)window_height, 0.1f, 100.0f);

    glm::vec3 light_color = glm::vec3(1.0f,1.0f,1.0f);
    Shader light_shader("./shaders/light/light.vs", "./shaders/light/light.fs");
    OpWindow::RegisterShader(&light_shader);
    light_shader.Use();

    glm::vec3 light_position = glm::vec3(2.0,2.0,4.0);
    glm::mat4 light_model_matrix = glm::mat4(1.0f);
    light_model_matrix = glm::translate(light_model_matrix, light_position);
    light_shader.SetMatrix("model_matrix", light_model_matrix);
    light_shader.SetMatrix("projection_matrix", projection_matrix);

    light_shader.SetVector3("light_color", light_color);

    Shader texture_shaded("./shaders/texture_shaded/texture_shaded.vs", "./shaders/texture_shaded/texture_shaded.fs");
    OpWindow::RegisterShader(&texture_shaded);
    texture_shaded.Use();

    texture_shaded.SetVector3("light.direction", glm::vec3(-0.2f,-1.0f,-0.3f));
    texture_shaded.SetVector3("light.ambient", glm::vec3(0.2f));
    texture_shaded.SetVector3("light.diffuse", glm::vec3(0.8f));
    texture_shaded.SetVector3("light.specular", glm::vec3(1.0f));
   
    texture_shaded.SetInt("material.diffuse", 0);
    texture_shaded.SetInt("material.specular", 1);
    texture_shaded.SetFloat("material.shininess", 32.0f);

    // model matrix
    glm::mat4 crate_model_matrix = glm::mat4(1.0f);
    crate_model_matrix = glm::translate(crate_model_matrix, glm::vec3(0.0,0.5,2.0));
    
    texture_shaded.SetMatrix("model_matrix", crate_model_matrix);
    texture_shaded.SetMatrix("projection_matrix", projection_matrix);

    glEnable(GL_DEPTH_TEST);
    
    float last_frame_time = static_cast<float>(glfwGetTime());

    UI::Init(OpWindow::GetWindowGLFW());

    // Game loop
    while(!glfwWindowShouldClose(OpWindow::GetWindowGLFW()))
    {
        float current_time = static_cast<float>(glfwGetTime());
        delta_time = current_time - last_frame_time;
        last_frame_time = current_time;

        // Input
        OpWindow::Tick();

        // Tick
        camera.Tick(delta_time);

        // Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        texture_shaded.Use();
        texture_shaded.SetMatrix("view_matrix", camera.GetViewMatrix());
        texture_shaded.SetVector3("camera_position", camera.GetPosition());
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // UI
        UI::BeginFrame();
        UI::ConfigureDebugWindow();
        UI::DrawFrame();

        OpWindow::EndFrame();
    }

    glfwTerminate();
    return 0;
}