#version 330 core

uniform vec3 u_color;

uniform vec3 u_light_position;
uniform vec3 u_camera_position;

// We store diffuse/specular map as sample2D, and ambient color is equal to diffuse color
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
};

struct AttenuationFactor {
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    // Angle which spot light cuts off
    float cut_off;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    AttenuationFactor attenuation;
};

uniform Material u_material;
uniform SpotLight u_light;

in vec3 vertex_normal;
in vec3 frag_position;
in vec2 texture_coord;

out vec4 FragColor;

void main() {
    float distance_to_light = length(u_light.position - frag_position);
    float attenuation = 1.0 / (
        u_light.attenuation.constant +
        (u_light.attenuation.linear * distance_to_light) +
        (u_light.attenuation.quadratic * pow(distance_to_light, 2))
    );

    vec3 ambient = u_light.ambient * texture(u_material.diffuse, texture_coord).rgb;
    ambient *= attenuation;

    vec3 obj_to_light_direction = normalize(u_light.position - frag_position);
    bool is_not_within_range = dot(obj_to_light_direction, normalize(-u_light.direction)) < u_light.cut_off;

    if (is_not_within_range) {
        FragColor = vec4(ambient, 1.0);
        return;
    }

    float diffuse_strength = max(dot(obj_to_light_direction, vertex_normal), .0);
    vec3 diffuse = u_light.diffuse * diffuse_strength * texture(u_material.diffuse, texture_coord).rgb;
    diffuse *= attenuation;

    vec3 obj_to_camera_direction = normalize(u_camera_position - frag_position);
    vec3 light_reflection = reflect(-obj_to_light_direction, vertex_normal);
    float specular_strength = pow(
        max(dot(light_reflection, obj_to_camera_direction), .0),
        u_material.shininess
    );
    vec3 specular = u_light.specular * specular_strength * texture(u_material.specular, texture_coord).rgb;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}