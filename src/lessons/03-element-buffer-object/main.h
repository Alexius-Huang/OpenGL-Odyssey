#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../libs/WindowManager.h"
#include "../libs/Shader.h"

using std::cout;
using std::endl;

namespace _03_ElementBufferObject {
    void main();
}

void _03_ElementBufferObject::main() {
    float width { 800 };
    float height { 600 };

    // We could draw two triangles using 6 vertices
    // Each triangle occupies 3 vertices
    // float vertices[] {
    //     // first triangle
    //     0.5f,  0.5f, 0.0f, // top right
    //     0.5f, -0.5f, 0.0f, // bottom right
    //    -0.5f,  0.5f, 0.0f, // top left
    //    // second triangle
    //     0.5f, -0.5f, 0.0f, // bottom right
    //    -0.5f, -0.5f, 0.0f, // bottom left
    //    -0.5f,  0.5f, 0.0f  // top left
    // };

    // But we could instead use Element Buffer Object to
    // specify which vertices to form triangle using indices
    float vertices[] = {
        0.5f,  0.5f, 0.0f, // top right
        0.5f, -0.5f, 0.0f, // bottom right
       -0.5f, -0.5f, 0.0f, // bottom left
       -0.5f,  0.5f, 0.0f  // top left
    };
    unsigned int indices[] = { // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    WindowManager manager { width, height };
    GLFWwindow* window { manager.init() };

    /* Putting data into buffer for GPU */
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO, EBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /* Compiling Shader Program */
    Shader shader_program {
        "./shaders/03-element-buffer-object/vertex.glsl",
        "./shaders/03-element-buffer-object/fragment.glsl"
    };

    /* Render Loop */
    manager.render([
        shader_program,
        VAO,
        EBO
    ] (WindowManager* context) {
        glClearColor(.2f, .3f, .3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Draw */
        shader_program.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    });

    /* Cleanup */
    glfwTerminate();
}
