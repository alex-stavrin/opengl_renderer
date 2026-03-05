#version 330 core

layout (location = 0) in vec3 input_position;
layout (location = 1) in vec3 input_normal;
layout (location = 2) in vec2 input_texture_coordinates;

out vec2 texture_coordinates;
out vec3 normal;
out vec3 fragment_position;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
    fragment_position = vec3(model_matrix * vec4(input_position, 1.0));
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(input_position, 1.0);
    texture_coordinates = input_texture_coordinates;

    // this is a way to convert the local normal to world normal (after any rotations
    // and scaling)
    normal = mat3(transpose(inverse(model_matrix))) * input_normal;
}