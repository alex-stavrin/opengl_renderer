#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const glm::vec3& n_position, float n_movement_speed, float n_mouse_sensitivity) : 
    position(n_position), up(glm::vec3(0,1,0)), yaw(0), pitch(0), front(0,0,1), movement_speed(n_movement_speed),
    mouse_sensitivity(n_mouse_sensitivity)
{
    world_up = up;
    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(position, position + front, up);
}

void Camera::UpdateCameraVectors()
{
    glm::vec3 new_front;
    new_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    new_front.y = sin(glm::radians(pitch));
    new_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(new_front);

    right = glm::normalize(glm::cross(front, world_up));
    up = glm::normalize(glm::cross(right, front));
}