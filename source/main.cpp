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
Shader* texture_shaded_pointer = nullptr;
bool flash_light_enabled = true;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_F && action == GLFW_PRESS && texture_shaded_pointer)
    {
        flash_light_enabled = !flash_light_enabled;
        
        // Update shader values only when they change
        float intensity = flash_light_enabled ? 1.0f : 0.0f;
        texture_shaded_pointer->Use();
        texture_shaded_pointer->SetVector3("spot_light.ambient", glm::vec3(intensity));
        texture_shaded_pointer->SetVector3("spot_light.diffuse", glm::vec3(intensity));
    }
}

int main()
{
    OpWindow::Init(glm::vec3(0.0f));

    float cube_vertices[] = 
    {
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
    
    Camera camera(OpWindow::GetWindowGLFW(), glm::vec3(0.0f), 3.0f, 0.05f, true);
    OpWindow::SetCamera(&camera);

    glm::vec3 crate_position = glm::vec3(0.0,0.5,2.0);
    glm::vec3 point_light_red_position = glm::vec3(-2.0,0.5, 4.0);
    glm::vec3 point_light_blue_position = glm::vec3(2.0,0.5, 4.0);
    glm::vec3 point_light_yellow_position = glm::vec3(-2.0,0.5, 0.0);
    glm::vec3 point_light_green_position = glm::vec3(2.0,0.5, 0.0);

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
    texture_shaded_pointer = &texture_shaded;
    OpWindow::RegisterShader(&texture_shaded);
    texture_shaded.Use();

    // directional light
    texture_shaded.SetVector3("directional_light.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
    texture_shaded.SetVector3("directional_light.ambient", glm::vec3(0.05f));
    texture_shaded.SetVector3("directional_light.diffuse", glm::vec3(0.4f));
    texture_shaded.SetVector3("directional_light.specular", glm::vec3(0.5));

    // point light 1
    texture_shaded.SetVector3("point_lights[0].position", point_light_red_position);
    texture_shaded.SetVector3("point_lights[0].ambient", glm::vec3(0.05f, 0.0f, 0.0f));
    texture_shaded.SetVector3("point_lights[0].diffuse", glm::vec3(0.8f, 0.0f, 0.0f));
    texture_shaded.SetVector3("point_lights[0].specular", glm::vec3(1.0f, 0.0f, 0.0f));
    texture_shaded.SetFloat("point_lights[0].constant", 1.0f);
    texture_shaded.SetFloat("point_lights[0].linear", 0.09f);
    texture_shaded.SetFloat("point_lights[0].quadratic", 0.032f);
    // point light 2
    texture_shaded.SetVector3("point_lights[1].position", point_light_blue_position);
    texture_shaded.SetVector3("point_lights[1].ambient", glm::vec3(0.0f, 0.0f, 0.05f));
    texture_shaded.SetVector3("point_lights[1].diffuse", glm::vec3(0.0f, 0.0f, 0.8f));
    texture_shaded.SetVector3("point_lights[1].specular", glm::vec3(0.0f, 0.0f, 1.0f));
    texture_shaded.SetFloat("point_lights[1].constant", 1.0f);
    texture_shaded.SetFloat("point_lights[1].linear", 0.09f);
    texture_shaded.SetFloat("point_lights[1].quadratic", 0.032f);
    // point light 3
    texture_shaded.SetVector3("point_lights[2].position", point_light_yellow_position);
    texture_shaded.SetVector3("point_lights[2].ambient", glm::vec3(0.05f, 0.05f, 0.0f));
    texture_shaded.SetVector3("point_lights[2].diffuse", glm::vec3(0.8f, 0.8f, 0.0f));
    texture_shaded.SetVector3("point_lights[2].specular", glm::vec3(1.0f, 1.0f, 0.0f));
    texture_shaded.SetFloat("point_lights[2].constant", 1.0f);
    texture_shaded.SetFloat("point_lights[2].linear", 0.09f);
    texture_shaded.SetFloat("point_lights[2].quadratic", 0.032f);
    // point light 4
    texture_shaded.SetVector3("point_lights[3].position", point_light_green_position);
    texture_shaded.SetVector3("point_lights[3].ambient", glm::vec3(0.0f, 0.05f, 0.0f));
    texture_shaded.SetVector3("point_lights[3].diffuse", glm::vec3(0.0f, 0.8f, 0.0f));
    texture_shaded.SetVector3("point_lights[3].specular", glm::vec3(0.0f, 1.0f, 0.0f));
    texture_shaded.SetFloat("point_lights[3].constant", 1.0f);
    texture_shaded.SetFloat("point_lights[3].linear", 0.09f);
    texture_shaded.SetFloat("point_lights[3].quadratic", 0.032f);

    // spotlight

    texture_shaded.SetVector3("spot_light.specular", glm::vec3(0.0f));
    texture_shaded.SetFloat("spot_light.constant", 1.0f);
    texture_shaded.SetFloat("spot_light.linear", 0.09f);
    texture_shaded.SetFloat("spot_light.quadratic", 0.032f);
    texture_shaded.SetFloat("spot_light.cutoff", glm::cos(glm::radians(12.5f)));
    texture_shaded.SetFloat("spot_light.outer_cutoff", glm::cos(glm::radians(15.0f)));
   
    texture_shaded.SetInt("material.diffuse", 0);
    texture_shaded.SetInt("material.specular", 1);
    texture_shaded.SetFloat("material.shininess", 32.0f);

    // model matrix
    glm::mat4 crate_model_matrix = glm::mat4(1.0f);
    crate_model_matrix = glm::translate(crate_model_matrix, crate_position);
    
    texture_shaded.SetMatrix("model_matrix", crate_model_matrix);
    texture_shaded.SetMatrix("projection_matrix", projection_matrix);

    glEnable(GL_DEPTH_TEST);
    
    float last_frame_time = static_cast<float>(glfwGetTime());

    UI::Init(OpWindow::GetWindowGLFW());

    texture_shaded.Use();
    texture_shaded.SetVector3("spot_light.ambient", glm::vec3(1.0f));
    texture_shaded.SetVector3("spot_light.diffuse", glm::vec3(1.0f));

    glfwSetKeyCallback(OpWindow::GetWindowGLFW(), key_callback);

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
        texture_shaded.SetVector3("spot_light.position", camera.GetPosition());
        texture_shaded.SetVector3("spot_light.direction", camera.GetFront());
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // LIGHTS
        light_shader.Use();
        light_shader.SetMatrix("view_matrix", camera.GetViewMatrix());

        // point light 1
        light_shader.SetVector3("light_color", glm::vec3(255.0,0.0,0.0));
        glm::mat4 light_model_matrix = glm::mat4(1.0f);
        light_model_matrix = glm::translate(light_model_matrix, point_light_red_position);
        texture_shaded.SetMatrix("model_matrix", light_model_matrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // point light 2
        light_shader.SetVector3("light_color", glm::vec3(0.0,0.0,255.0));
        light_model_matrix = glm::mat4(1.0f);
        light_model_matrix = glm::translate(light_model_matrix, point_light_blue_position);
        texture_shaded.SetMatrix("model_matrix", light_model_matrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // point light 3
        light_shader.SetVector3("light_color", glm::vec3(255.0,255.0,0.0));
        light_model_matrix = glm::mat4(1.0f);
        light_model_matrix = glm::translate(light_model_matrix, point_light_yellow_position);
        texture_shaded.SetMatrix("model_matrix", light_model_matrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // point light 4
        light_shader.SetVector3("light_color", glm::vec3(0.0, 255.0,0.0));
        light_model_matrix = glm::mat4(1.0f);
        light_model_matrix = glm::translate(light_model_matrix, point_light_green_position);
        texture_shaded.SetMatrix("model_matrix", light_model_matrix);
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