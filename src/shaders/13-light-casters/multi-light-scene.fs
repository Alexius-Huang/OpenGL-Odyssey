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

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct AttenuationFactor {
    float constant;
    float linear;
    float quadratic;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    AttenuationFactor attenuation;
};

uniform Material u_material;
uniform DirectionalLight u_directional_light;

#define POINT_LIGHTS_COUNT 4
uniform PointLight u_point_lights[POINT_LIGHTS_COUNT];

in vec3 vertex_normal;
in vec3 frag_position;
in vec2 texture_coord;

out vec4 FragColor;

vec3 calculate_directional_light(DirectionalLight light, vec3 normal, vec3 view_direction) {
    vec3 ambient = light.ambient * texture(u_material.diffuse, texture_coord).rgb;

    vec3 obj_to_light_direction = normalize(-light.direction);

    float diffuse_strength = max(dot(obj_to_light_direction, normal), .0);
    vec3 diffuse = light.diffuse * diffuse_strength * texture(u_material.diffuse, texture_coord).rgb;

    vec3 light_reflection = reflect(-obj_to_light_direction, normal);
    float specular_strength = pow(
        max(dot(light_reflection, view_direction), .0),
        u_material.shininess
    );
    vec3 specular = light.specular * specular_strength * texture(u_material.specular, texture_coord).rgb;    

    return ambient + diffuse + specular;
}

vec3 calculate_point_light(PointLight light, vec3 normal, vec3 fragment_position, vec3 view_direction) {
    float distance_to_light = length(light.position - fragment_position);
    float attenuation = 1.0 / (
        light.attenuation.constant +
        (light.attenuation.linear * distance_to_light) +
        (light.attenuation.quadratic * pow(distance_to_light, 2))
    );

    vec3 ambient = light.ambient * texture(u_material.diffuse, texture_coord).rgb;
    ambient *= attenuation;

    vec3 obj_to_light_direction = normalize(light.position - fragment_position);
    float diffuse_strength = max(dot(obj_to_light_direction, normal), .0);
    vec3 diffuse = light.diffuse * diffuse_strength * texture(u_material.diffuse, texture_coord).rgb;
    diffuse *= attenuation;

    vec3 light_reflection = reflect(-obj_to_light_direction, normal);
    float specular_strength = pow(
        max(dot(light_reflection, view_direction), .0),
        u_material.shininess
    );
    vec3 specular = light.specular * specular_strength * texture(u_material.specular, texture_coord).rgb;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

void main() {
    vec3 obj_to_camera_direction = normalize(u_camera_position - frag_position);

    vec3 result = calculate_directional_light(
        u_directional_light,
        vertex_normal,
        obj_to_camera_direction
    );

    for (int i = 0; i < POINT_LIGHTS_COUNT; i++) {
        result += calculate_point_light(
            u_point_lights[i],
            vertex_normal,
            frag_position,
            obj_to_camera_direction
        );
    }

    // result += calculate_spot_light(...);

    FragColor = vec4(result, 1.0);
}
