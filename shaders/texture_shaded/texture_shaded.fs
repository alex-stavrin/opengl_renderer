#version 330 core

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec2 texture_coordinates;
in vec3 normal;
in vec3 fragment_position;

uniform vec3 camera_position;

uniform Material material;

uniform Light light;

void main()
{
    vec4 diffuse_texture = texture(material.diffuse, texture_coordinates);
    vec3 object_color = diffuse_texture.rgb;

    // ambient
    vec3 ambient = light.ambient * object_color;

    // diffuse
    vec3 actual_normal = normalize(normal);
    vec3 light_direction = normalize(light.position - fragment_position);
    float diffuse_amount = max(dot(actual_normal, light_direction), 0.0);
    vec3 diffuse = diffuse_amount * object_color * light.diffuse;

    // specular
    vec3 camera_direction = normalize(camera_position - fragment_position);
    vec3 reflection_direction = reflect(-light_direction, actual_normal);
    float specular_amount = pow(max(dot(camera_direction, reflection_direction),
        0.0), material.shininess);
    vec3 specular_texture = vec3(texture(material.specular, texture_coordinates));
    vec3 specular =  (specular_amount * specular_texture) * light.specular;

    // final
    vec3 final_color = ambient + diffuse + specular;
    FragColor = vec4(final_color, 1.0);
}