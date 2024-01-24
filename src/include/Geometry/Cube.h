#ifndef _GEOMETRY_CUBE_H_
#define _GEOMETRY_CUBE_H_

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Shader/Shader.h>
#include <Transform/Transform.h>

namespace Geometry {
    class Cube;

    float CUBE_VERTICES[] {
        /* Positions */
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
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

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
    };

    size_t CUBE_VERTICES_SIZE { sizeof(CUBE_VERTICES) };
    size_t CUBE_VERTICES_COUNT { sizeof(CUBE_VERTICES) / sizeof(CUBE_VERTICES[0]) };
    bool CUBE_VBO_FILLED { false };
    unsigned int CUBE_VBO;
}

class Geometry::Cube {
private:
    unsigned int VAO;
    Shader shader_program;

public:
    Cube(): shader_program { Shader {
        "./shaders/Geometry/Cube/Cube.vs",
        "./shaders/Geometry/Cube/Cube.fs"
    } } {
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

        this->shader_program.use();
        this->shader_program.set_vec3("u_color", 1.0f, 1.0f, 1.0f);
    }

    void set_color(glm::vec3 color_vec) const {
        this->shader_program.set_vec3("u_color", color_vec);
    }
    void set_color(float r, float g, float b) const {
        this->shader_program.set_vec3("u_color", r, g, b);
    }

    void set_coordinate_system(glm::mat4 model, glm::mat4 view, glm::mat4 projection) const {
        this->shader_program.set_mat4("model", model);
        this->shader_program.set_mat4("view", view);
        this->shader_program.set_mat4("projection", projection);
    }

    void render() const {
        this->shader_program.use();

        glBindVertexArray(this->VAO);
        glDrawArrays(GL_TRIANGLES, 0, Geometry::CUBE_VERTICES_COUNT);
    }
};

#endif
