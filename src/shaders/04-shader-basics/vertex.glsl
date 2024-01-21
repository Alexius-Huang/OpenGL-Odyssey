#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_vertex_color;

out vec3 vertex_color;

uniform float u_x_offset;
uniform float u_y_offset;

void main() {
    gl_Position = vec4(
        a_position.x + u_x_offset,
        a_position.y + u_y_offset,
        a_position.z,
        1.0
    );
    vertex_color = a_vertex_color;
}
