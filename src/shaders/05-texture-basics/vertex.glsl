#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_vertex_color;
layout (location = 2) in vec2 a_texture_coord;

out vec3 vertex_color;
out vec2 texture_coord;

void main() {
    gl_Position = vec4(a_position, 1.0);
    vertex_color = a_vertex_color;
    texture_coord = a_texture_coord;
}
