#ifndef _GEOMETRY_CUBE_CONST_H_
#define _GEOMETRY_CUBE_CONST_H_

#include <cctype>

namespace Geometry {
    float CUBE_VERTICES[] {
        /* Positions */
         0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
    };

    float CUBE_TEXTURE_COORDS[] {
        1.0f,  0.0f,
        0.0f,  0.0f,
        1.0f,  1.0f,
        0.0f,  1.0f,
        1.0f,  1.0f,
        0.0f,  0.0f,

        0.0f,  0.0f,
        1.0f,  0.0f,
        1.0f,  1.0f,
        1.0f,  1.0f,
        0.0f,  1.0f,
        0.0f,  0.0f,

        1.0f,  0.0f,
        1.0f,  1.0f,
        0.0f,  1.0f,
        0.0f,  1.0f,
        0.0f,  0.0f,
        1.0f,  0.0f,

        1.0f,  1.0f,
        1.0f,  0.0f,
        0.0f,  1.0f,
        0.0f,  0.0f,
        0.0f,  1.0f,
        1.0f,  0.0f,

        0.0f,  1.0f,
        1.0f,  1.0f,
        1.0f,  0.0f,
        1.0f,  0.0f,
        0.0f,  0.0f,
        0.0f,  1.0f,

        1.0f,  1.0f,
        0.0f,  1.0f,
        1.0f,  0.0f,
        0.0f,  0.0f,
        1.0f,  0.0f,
        0.0f,  1.0f
    };

    size_t CUBE_VERTICES_SIZE { sizeof(CUBE_VERTICES) };
    size_t CUBE_VERTICES_COUNT { sizeof(CUBE_VERTICES) / sizeof(CUBE_VERTICES[0]) };

    size_t CUBE_TEXTURE_COORD_SIZE { sizeof(CUBE_TEXTURE_COORDS) };
    size_t CUBE_TEXTURE_COORD_COUNT { sizeof(CUBE_TEXTURE_COORDS) / sizeof(CUBE_TEXTURE_COORDS[0]) };

    bool CUBE_VBO_FILLED { false };
    unsigned int CUBE_VBO;
}

#endif
