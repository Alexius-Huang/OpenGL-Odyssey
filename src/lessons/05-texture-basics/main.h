#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <Shader/Shader.h>
#include <cmath>
#include <WindowManager/WindowManager.h>

using std::cout;
using std::endl;

namespace _05_TextureBasics {
    void main();
}

void _05_TextureBasics::main() {
    float width { 800 };
    float height { 600 };

    const float vertices[] = {
        /* Positions */    /* Colors */        /* Texture 2D */
         .5f,  .5f, .0f,   1.0f,  .0f,  .0f,   1.0f, 1.0f,
         .5f, -.5f, .0f,    .0f, 1.0f,  .0f,   1.0f,  .0f,
        -.5f, -.5f, .0f,    .0f,  .0f, 1.0f,    .0f,  .0f,
        -.5f,  .5f, .0f,   1.0f, 1.0f,  .0f,    .0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    WindowManager manager { width, height };
    manager.init();

    /* Putting data to the buffer for GPU */
    unsigned int VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLsizei stride { 8 * sizeof(float) };

    // Location = 0, positions data, stride is 8
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*) 0);
    glEnableVertexAttribArray(0);

    // Location = 1, colors data, offset is 3
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Location = 2, texture coordinate, offset is 6
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Element Buffer Object
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /* Shader */
    Shader shader_program {
        "./shaders/05-texture-basics/vertex.glsl",
        "./shaders/05-texture-basics/fragment.glsl"
    };

    /* Texture */
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Loading texture
    int texture_width, texture_height, nr_channels;
    unsigned char *data = stbi_load(
        "./resources/textures/container.jpg",
        &texture_width,
        &texture_height,
        &nr_channels,
        0
    );

    glTexImage2D(
        /* texture target */                  GL_TEXTURE_2D,
        /* mipmapping level */                0,
        /* texture format */                  GL_RGB,
        /* texture dimension */               texture_width, texture_height,
        /* legacy stuff... */                 0,
        /* format, data type of source img */ GL_RGB, GL_UNSIGNED_BYTE,
        /* actual image */                    data
    );
    glGenerateMipmap(GL_TEXTURE_2D);
    // clear the image data after texture being generated
    stbi_image_free(data);

    manager.render([
        EBO,
        VAO,
        shader_program,
        texture
    ] (WindowManager* context) {
        glClearColor(.2f, .3f, .3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, texture);
        shader_program.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    });

    glfwTerminate();
}
