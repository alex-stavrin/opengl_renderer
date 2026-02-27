#version 330 core

out vec4 FragColor;

in vec3 vertex_color;
in vec2 texture_coordinates;

uniform sampler2D our_texture;

void main()
{
    FragColor = texture(our_texture, texture_coordinates);
}