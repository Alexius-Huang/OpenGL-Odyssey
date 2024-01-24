#version 330 core

uniform vec3 u_color;

uniform vec3 u_light_position;
uniform vec3 u_camera_position;

struct Material {
    vec3  ambient;
    vec3  diffuse;
    vec3  specular;
    float shininess;
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

out vec4 FragColor;

void main() {
    vec3 ambient = u_light.ambient * u_material.ambient;

    vec3 obj_to_light_direction = normalize(u_light.position - frag_position);
    float diffuse_strength = max(dot(obj_to_light_direction, vertex_normal), .0);
    vec3 diffuse = u_light.diffuse * (diffuse_strength * u_material.diffuse);

    vec3 obj_to_camera_direction = normalize(u_camera_position - frag_position);
    vec3 light_reflection = reflect(-obj_to_light_direction, vertex_normal);
    float specular_strength = pow(
        max(dot(light_reflection, obj_to_camera_direction), .0),
        u_material.shininess
    );
    vec3 specular = u_light.specular * (specular_strength * u_material.specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
