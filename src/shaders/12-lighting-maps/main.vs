#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_vertex_normal;
layout (location = 2) in vec2 a_texture_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertex_normal;
out vec3 frag_position;
out vec2 texture_coord;

void main() {
    gl_Position = projection * view * model * vec4(a_position, 1.0);
    texture_coord = a_texture_coord;

    // If we do not transform the normal, normal will still be fixed when object
    // being scaled or rotated
    //
    // inversion of matrix is costly, it is recommended to calculated in CPU
    mat3 normal_transform = mat3(transpose(inverse(model)));
    vertex_normal = normalize(normal_transform * a_vertex_normal);

    // If we do not transform to world position, it will be fixed lighting
    frag_position = vec3(model * vec4(a_position, 1.0));
}
