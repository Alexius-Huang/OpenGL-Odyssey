#ifndef _GEOMETRY_CUBE_V2_H_
#define _GEOMETRY_CUBE_V2_H_

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Shader/Shader.h>
#include <Transform/Transform.h>
#include "./Cube.const.h"

namespace Geometry {
    class CubeV2;
}

class Geometry::CubeV2 {
private:
    unsigned int VAO;

public:
    CubeV2() {
        glGenVertexArrays(1, &(this->VAO));
        glBindVertexArray(this->VAO);

        if (!Geometry::CUBE_VBO_FILLED) {
            glGenBuffers(1, &(Geometry::CUBE_VBO));
            glBindBuffer(GL_ARRAY_BUFFER, Geometry::CUBE_VBO);
            glBufferData(GL_ARRAY_BUFFER, Geometry::CUBE_VERTICES_SIZE, Geometry::CUBE_VERTICES, GL_STATIC_DRAW);

            Geometry::CUBE_VBO_FILLED = true;
        } else {
            glBindBuffer(GL_ARRAY_BUFFER, Geometry::CUBE_VBO);
        }

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
        glEnableVertexAttribArray(0);
    }

    void render() const {
        glBindVertexArray(this->VAO);
        glDrawArrays(GL_TRIANGLES, 0, Geometry::CUBE_VERTICES_COUNT);
    }
};

#endif
