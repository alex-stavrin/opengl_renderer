#version 330 core

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

out vec4 FragColor;

in vec2 texture_coordinates;
in vec3 normal;
in vec3 fragment_position;

uniform sampler2D texture0;
uniform sampler2D texture1;

uniform vec3 light_color;
uniform vec3 light_position;
uniform vec3 camera_position;

uniform Material material;

void main()
{
    vec4 texture = mix(texture(texture0, texture_coordinates), texture(texture1, texture_coordinates), 0.2);
    vec3 object_color = texture.rgb;

    // ambient
    vec3 ambient = light_color * material.ambient;

    // diffuse
    vec3 actual_normal = normalize(normal);
    vec3 light_direction = normalize(light_position - fragment_position);
    float diffuse_amount = max(dot(actual_normal, light_direction), 0.0);
    vec3 diffuse = (diffuse_amount * material.diffuse) * light_color;

    // specular
    vec3 camera_direction = normalize(camera_position - fragment_position);
    vec3 reflection_direction = reflect(-light_direction, actual_normal);
    float specular_amount = pow(max(dot(camera_direction, reflection_direction),
        0.0), material.shininess);
    vec3 specular =  (specular_amount * material.specular) * light_color;

    // final
    vec3 final_color = object_color * (ambient + diffuse + specular);
    FragColor = vec4(final_color, 1.0);
}