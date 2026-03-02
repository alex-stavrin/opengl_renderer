#pragma once

#include "glm/glm.hpp"
#include "GLFW/glfw3.h"

class Camera
{
    private:
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;

        glm::vec3 world_up;

        float yaw;
        float pitch;

        float movement_speed;
        float mouse_sensitivity;

        void UpdateCameraVectors();

        GLFWwindow* window;

        bool first_mouse = true;
        float last_mouse_y;
        float last_mouse_x;

    public:

        Camera(GLFWwindow* n_window, const glm::vec3& n_position, float n_movement_speed, float n_mouse_sensitivity);
        glm::mat4 GetViewMatrix();
        void Tick(float delta_time);
        void OnMouseMoved(GLFWwindow* window, double x_pos_in, double y_pos_in);
};