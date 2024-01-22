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
    void load_texture(const char* path, GLenum format);
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
    unsigned int texture_1;
    glGenTextures(1, &texture_1);
    glActiveTexture(GL_TEXTURE0); // activate texture unit
    glBindTexture(GL_TEXTURE_2D, texture_1);

    // Set texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Loading texture
    load_texture("./resources/textures/container.jpg", GL_RGB);

    unsigned int texture_2;
    glGenTextures(1, &texture_2);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_2);

    // Set texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Loading texture
    load_texture("./resources/textures/awesomeface.png", GL_RGBA);

    // We need to assign texture to the shader uniform via texture unit
    shader_program.use();
    shader_program.set_int("u_texture_1", 0);
    shader_program.set_int("u_texture_2", 1);

    float* opacity = new float { .5f };
    manager.listen(GLFW_KEY_UP, [opacity] (WindowManager* context) {
        *opacity += .01;
        if (*opacity > 1.) {
            *opacity = 1.;
        }
    });
    manager.listen(GLFW_KEY_DOWN, [opacity] (WindowManager* context) {
        *opacity -= .01;
        if (*opacity < .0) {
            *opacity = .0;
        }
    });

    manager.render([
        EBO,
        VAO,
        shader_program,
        texture_1,
        texture_2,
        opacity
    ] (WindowManager* context) {
        glClearColor(.2f, .3f, .3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture_2);

        shader_program.use();
        shader_program.set_float("opacity", *opacity);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    });

    delete opacity;
    glfwTerminate();
}

void _05_TextureBasics::load_texture(const char* path, GLenum format) {
    // Loading texture
    int texture_width, texture_height, nr_channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(
        path,
        &texture_width,
        &texture_height,
        &nr_channels,
        0
    );

    if (!data) {
        cout << "Failed to load texture: " << path << endl;
        return;
    }

    glTexImage2D(
        /* texture target */                  GL_TEXTURE_2D,
        /* mipmapping level */                0,
        /* texture format */                  GL_RGB,
        /* texture dimension */               texture_width, texture_height,
        /* legacy stuff... */                 0,
        /* format, data type of source img */ format, GL_UNSIGNED_BYTE,
        /* actual image */                    data
    );
    glGenerateMipmap(GL_TEXTURE_2D);
    // clear the image data after texture being generated
    stbi_image_free(data);
}
