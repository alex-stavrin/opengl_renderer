#version 330 core

out vec4 FragColor;

in vec3 vertex_color;
in vec2 texture_coordinates;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
    FragColor = mix(texture(texture0, texture_coordinates), texture(texture1, texture_coordinates), 0.2);
}