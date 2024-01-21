#version 330 core

in vec3 vertex_color;
in vec2 texture_coord;

/*
 *  This location of a texture is more commonly known as a texture unit.
 *  
 *  The default texture unit for a texture is 0 which is the default active
 *  texture unit so we didn’t need to assign a location
 */
uniform sampler2D u_texture;

out vec4 FragColor;

void main() {
    FragColor = texture(u_texture, texture_coord) * vec4(vertex_color, 1.0);
}
