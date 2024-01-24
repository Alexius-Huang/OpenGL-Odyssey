#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_vertex_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 u_light_source_position;

out vec3 vertex_normal;
out vec3 frag_position;
out vec3 light_source_position;

void main() {
    gl_Position = projection * view * model * vec4(a_position, 1.0);

    // When transforming to view space we would need to apply
    // transform to vertex and light source
    mat3 normal_transform = mat3(transpose(inverse(view * model)));
    vertex_normal = normalize(normal_transform * a_vertex_normal);

    frag_position = vec3(view * model * vec4(a_position, 1.0));
    light_source_position = vec3(view * vec4(u_light_source_position, 1.0));
}
