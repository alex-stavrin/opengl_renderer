#version 330 core

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirectionalLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    
    float cutoff;
    float outer_cutoff;

    float constant;
    float linear;
    float quadratic;

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

uniform DirectionalLight directional_light;
uniform PointLight point_lights[4];
uniform SpotLight spot_light;

vec3 CalculateDirectionalLight(vec3 texture_color, vec3 texture_specular, DirectionalLight directional_light, vec3 normal,
    vec3 view_direction)
{
    vec3 light_direction = normalize(-directional_light.direction);
    float diffuse_amount = max(dot(normal, light_direction), 0.0);
    vec3 reflection_direction = reflect(-light_direction, normal);
    float specular_amount = pow(max(dot(view_direction, reflection_direction), 0.0), material.shininess);

    vec3 ambient = directional_light.ambient * texture_color;
    vec3 diffuse = directional_light.diffuse * diffuse_amount * texture_color;
    vec3 specular = directional_light.specular * specular_amount * texture_specular;
    return ambient + diffuse + specular;
}

vec3 CalculatePointLight(vec3 texture_color, vec3 texture_specular, PointLight point_light, vec3 normal,
    vec3 fragment_position, vec3 view_direction)
{
    vec3 light_direction = normalize(point_light.position - fragment_position);
    float diffuse_amount = max(dot(normal, light_direction), 0.0);

    vec3 reflection_direction = reflect(-light_direction, normal);
    float specular_amount = pow(max(dot(view_direction, reflection_direction), 0.0), material.shininess);

    float distance = length(point_light.position - fragment_position);
    float attenuation = 1.0 / (point_light.constant + point_light.linear * distance + point_light.quadratic * (distance * distance));

    vec3 ambient = point_light.ambient * texture_color;
    vec3 diffuse = point_light.diffuse * diffuse_amount * texture_color;
    vec3 specular = point_light.specular * specular_amount * texture_specular;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

vec3 CalculateSpotLight(vec3 texture_color, vec3 texture_specular, SpotLight spot_light, vec3 normal, vec3 fragment_position,
    vec3 view_direction)
{
    vec3 light_direction = normalize(spot_light.position - fragment_position);
    float diffuse_amount = max(dot(normal, light_direction), 0.0);

    vec3 reflection_direction = reflect(-light_direction, normal);
    float specular_amount = pow(max(dot(view_direction, reflection_direction), 0.0), material.shininess);

    float distance = length(spot_light.position - fragment_position);
    float attenuation = 1.0 / (spot_light.constant + spot_light.linear * distance + spot_light.quadratic * (distance * distance));

    float theta = dot(light_direction, normalize(-spot_light.direction));
    float epsilon = spot_light.cutoff - spot_light.outer_cutoff;
    float intensity = clamp((theta - spot_light.outer_cutoff) / epsilon, 0.0, 1.0);

    vec3 ambient = spot_light.ambient * texture_color;
    vec3 diffuse = spot_light.diffuse * diffuse_amount * texture_color;
    vec3 specular = spot_light.specular * specular_amount * texture_specular;

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return ambient + diffuse + specular;
}

void main()
{
    vec4 diffuse_texture = texture(material.diffuse, texture_coordinates);
    vec3 texture_color = diffuse_texture.rgb;

    vec4 specular_texture = texture(material.specular, texture_coordinates);
    vec3 texture_specular = specular_texture.rgb;

    vec3 actual_normal = normalize(normal);
    vec3 view_direction = normalize(camera_position - fragment_position);

    vec3 result = CalculateDirectionalLight(texture_color, texture_specular,
    directional_light, actual_normal, view_direction);

    for(int i = 0; i < 4; i++)
    {
        result += CalculatePointLight(texture_color, texture_specular, point_lights[i],
            actual_normal, fragment_position, view_direction);
    }

    result += CalculateSpotLight(texture_color, texture_specular, spot_light,
    actual_normal, fragment_position, view_direction);

    FragColor = vec4(result, 1.0);
}