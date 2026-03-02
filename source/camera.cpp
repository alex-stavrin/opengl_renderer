#include <iostream>

#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

Camera::Camera(GLFWwindow* n_window, const glm::vec3& n_position,
    float n_movement_speed, float n_mouse_sensitivity, bool n_invert_y_axis) : 
    window(n_window), position(n_position), up(glm::vec3(0,1,0)), yaw(0), pitch(0),
    front(0,0,1), movement_speed(n_movement_speed), mouse_sensitivity(n_mouse_sensitivity),
    invert_y_axis(n_invert_y_axis)
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

    right = glm::normalize(glm::cross(world_up, front));
    up = glm::normalize(glm::cross(front, right));
}

void Camera::Tick(float delta_time)
{
    if (window == nullptr) return;

    // Movement
    glm::vec3 movement_direction(0.0, 0.0, 0.0);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        movement_direction += front;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        movement_direction += -front;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        movement_direction += right;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        movement_direction += -right;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        movement_direction += world_up;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        movement_direction += -world_up;
    }

    float movement_direction_length = glm::length(movement_direction);
    if(movement_direction_length > 0.001)
    {
        const float camera_speed = movement_speed * delta_time;
        movement_direction = glm::normalize(movement_direction);

        position += movement_direction * camera_speed;
    }
}

void Camera::OnMouseMoved(GLFWwindow* window, double x_pos_in, double y_pos_in)
{
    float current_mouse_x = static_cast<float>(x_pos_in);
    float current_mouse_y = static_cast<float>(y_pos_in);

    if (first_mouse)
    {
        last_mouse_x = current_mouse_x;
        last_mouse_y = current_mouse_y;
        first_mouse = false;
    }

    float mouse_offset_x = current_mouse_x - last_mouse_x;
    float mouse_offset_y = current_mouse_y - last_mouse_y;
    if (invert_y_axis) mouse_offset_y = -mouse_offset_y;

    mouse_offset_x *= mouse_sensitivity;
    mouse_offset_y *= mouse_sensitivity;

    yaw += mouse_offset_x;
    pitch += mouse_offset_y;

    if(pitch > 89.0f) pitch = 89.0f;
    if(pitch < -89.0f) pitch = -89.0f;

    UpdateCameraVectors();

    last_mouse_x = current_mouse_x;
    last_mouse_y = current_mouse_y;
}