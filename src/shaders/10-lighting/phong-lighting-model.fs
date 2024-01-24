#version 330 core

uniform vec3 u_light_color;
uniform vec3 u_color;
uniform vec3 u_light_source_position;
uniform vec3 u_camera_position;

uniform float u_ambient_intensity;
uniform float u_specular_intensity;
uniform int u_specular_shininess;

in vec3 vertex_normal;
in vec3 frag_position;

out vec4 FragColor;

void main() {
    vec3 ambient_light = u_ambient_intensity * u_light_color;

    vec3 obj_to_light_source = normalize(u_light_source_position - frag_position);
    float diffuse_intensity = max(dot(obj_to_light_source, vertex_normal), 0);
    vec3 diffuse_light = diffuse_intensity * u_light_color;

    // reflect vector expect to point from light source to object, hence we
    // must invert the vector
    vec3 reflected = reflect(-obj_to_light_source, vertex_normal);

    vec3 obj_to_camera_position = normalize(u_camera_position - frag_position);
    float specular_strength = pow(max(dot(reflected, obj_to_camera_position), .0), u_specular_shininess);
    vec3 specular_light = u_specular_intensity * specular_strength * u_light_color;

    vec3 result = (ambient_light + diffuse_light + specular_light) * u_color;
    FragColor = vec4(result, 1.0);
}
