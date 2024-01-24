#version 330 core

uniform vec3 u_color;

uniform vec3 u_light_position;
uniform vec3 u_camera_position;

// We store diffuse/specular map as sample2D, and ambient color is equal to diffuse color
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;

    // Emission map is to simulate if the object can glow    
    sampler2D emission;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material u_material;
uniform Light u_light;

in vec3 vertex_normal;
in vec3 frag_position;
in vec2 texture_coord;

out vec4 FragColor;

void main() {
    vec3 ambient = u_light.ambient * texture(u_material.diffuse, texture_coord).rgb;

    vec3 obj_to_light_direction = normalize(u_light.position - frag_position);
    float diffuse_strength = max(dot(obj_to_light_direction, vertex_normal), .0);
    vec3 diffuse = u_light.diffuse * diffuse_strength * texture(u_material.diffuse, texture_coord).rgb;

    vec3 obj_to_camera_direction = normalize(u_camera_position - frag_position);
    vec3 light_reflection = reflect(-obj_to_light_direction, vertex_normal);
    float specular_strength = pow(
        max(dot(light_reflection, obj_to_camera_direction), .0),
        u_material.shininess
    );
    vec3 specular = u_light.specular * specular_strength * texture(u_material.specular, texture_coord).rgb;

    vec3 emission = texture(u_material.emission, texture_coord).rgb;

    vec3 result = ambient + diffuse + specular + emission;
    FragColor = vec4(result, 1.0);
}
