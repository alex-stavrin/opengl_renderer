#version 330 core

layout (location = 0) in vec3 input_position;
layout (location = 1) in vec3 input_color;
layout (location = 2) in vec2 input_texture_coordinates;

out vec3 vertex_color;
out vec2 texture_coordinates;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(input_position, 1.0);
    vertex_color = input_color;
    texture_coordinates = input_texture_coordinates;
}