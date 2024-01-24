#version 330 core

in vec2 texture_coord;

/*
 *  This location of a texture is more commonly known as a texture unit.
 *  
 *  The default texture unit for a texture is 0 which is the default active
 *  texture unit so we didn’t need to assign a location unless we have more
 *  than 1 textures
 */
uniform sampler2D u_texture_1;
uniform sampler2D u_texture_2;

uniform float opacity;

out vec4 FragColor;

void main() {
    FragColor = mix(
        texture(u_texture_1, texture_coord),
        texture(u_texture_2, texture_coord),
        opacity
    );
}
