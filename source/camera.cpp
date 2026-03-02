#include <iostream>

#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

Camera::Camera(GLFWwindow* n_window, const glm::vec3& n_position, float n_movement_speed, float n_mouse_sensitivity) : 
    window(n_window), position(n_position), up(glm::vec3(0,1,0)), yaw(0), pitch(0), front(0,0,1),
    movement_speed(n_movement_speed), mouse_sensitivity(n_mouse_sensitivity)
{
    world_up = up;
    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAtLH(position, position + front, up);
}

void Camera::UpdateCameraVectors()
{
    glm::vec3 new_front;
    new_front.x = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    new_front.y = sin(glm::radians(pitch));
    new_front.z = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(new_front);
    std::cout << "Front: " << glm::to_string(front) << std::endl;

    right = glm::normalize(glm::cross(world_up, front));
    std::cout << "Right: " << glm::to_string(right) << std::endl;
    up = glm::normalize(glm::cross(front, right));
    std::cout << "Up: " << glm::to_string(up) << std::endl;
}

void Camera::Tick(float delta_time)
{
    if (window == nullptr) return;

    const float camera_speed = movement_speed * delta_time;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        position += front * camera_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        position += -front * camera_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        position += right * camera_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        position += -right * camera_speed;
    } 
}