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
    vec3 direction;
    float cutoff;
    float outer_cutoff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
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

    vec3 actual_normal = normalize(normal);
    vec3 light_direction = normalize(light.position - fragment_position);

    vec3 specular = vec3(0.0f);
    vec3 diffuse = vec3(0.0f);

    float theta = dot(light_direction, normalize(-light.direction));
    float epsilon = light.cutoff - light.outer_cutoff;
    float spotlight_intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0, 1.0);

    // diffuse 
    float diffuse_amount = max(dot(actual_normal, light_direction), 0.0);
    diffuse = diffuse_amount * object_color * light.diffuse;

    // specular
    vec3 camera_direction = normalize(camera_position - fragment_position);
    vec3 reflection_direction = reflect(-light_direction, actual_normal);
    float specular_amount = pow(max(dot(camera_direction, reflection_direction),
        0.0), material.shininess);
    vec3 specular_texture = vec3(texture(material.specular, texture_coordinates));
    specular =  (specular_amount * specular_texture) * light.specular;


    // point light attenuation
    float distance = length(light.position - fragment_position);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation * spotlight_intensity;
    specular *= attenuation * spotlight_intensity;

    // final
    vec3 final_color = ambient + diffuse + specular;
    FragColor = vec4(final_color, 1.0);
}