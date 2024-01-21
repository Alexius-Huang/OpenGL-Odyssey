#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include "../libs/WindowManager.h"
#include "../libs/Shader.h"

using std::cout;
using std::endl;

namespace _04_ShaderBasics {
    void main();
}

void _04_ShaderBasics::main() {
    float width { 800 };
    float height { 600 };

    // Assume we would like to draw a triangle which outputs color
    // in different vertices, we would need to pass in data into shader
    float vertices[] = {
       // positions         // colors
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
       -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
        0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // top
   };

    WindowManager manager { width, height };
    manager.init();

    /* Putting data into buffer for CPU */
    unsigned int VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Location = 0, which has 3 float data represent vertex position, stride is size = 6
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    // Location = 1, which also has 3 float data represent vertex color, starting offset with 3
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    /* Compiling Shader */
    Shader shader_program {
        "./shaders/04-shader-basics/vertex.glsl",
        "./shaders/04-shader-basics/fragment.glsl"
    };

    /* Render Loop */
    manager.render([
        VAO,
        shader_program
    ] (WindowManager* context) {
        glClearColor(.2f, .3f, .3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader_program.use();

        // Setting uniforms to animate
        int x_offset_uniform_location = glGetUniformLocation(shader_program.ID, "u_x_offset");
        glUniform1f(x_offset_uniform_location, cos((float)glfwGetTime()) * .3);
        int y_offset_uniform_location = glGetUniformLocation(shader_program.ID, "u_y_offset");
        glUniform1f(y_offset_uniform_location, sin((float)glfwGetTime()) * .3);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 9);
    });

    /* Clean Up */
    glfwTerminate();
}
