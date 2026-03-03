#version 330 core

out vec4 FragColor;

in vec2 texture_coordinates;
in vec3 normal;
in vec3 fragment_position;

uniform sampler2D texture0;
uniform sampler2D texture1;

uniform vec3 light_color;
uniform vec3 light_position;
uniform vec3 camera_position;

void main()
{
    vec4 texture = mix(texture(texture0, texture_coordinates), texture(texture1, texture_coordinates), 0.2);
    vec3 object_color = texture.rgb;

    // ambient
    float ambient_strength = 0.2;
    vec3 ambient = light_color * ambient_strength;

    // diffuse
    vec3 actual_normal = normalize(normal);
    vec3 light_direction = normalize(light_position - fragment_position);
    float diffuse_amount = max(dot(actual_normal, light_direction), 0.0);
    vec3 diffuse = diffuse_amount * light_color;

    // specular
    float specular_strength = 0.5;
    vec3 camera_direction = normalize(camera_position - fragment_position);
    vec3 reflection_direction = reflect(-light_direction, actual_normal);
    float specular_amount = pow(max(dot(camera_direction, reflection_direction), 0.0), 32);
    vec3 specular = specular_strength * specular_amount * light_color;

    // final
    vec3 final_color = object_color * (ambient + diffuse + specular);
    FragColor = vec4(final_color, 1.0);
}