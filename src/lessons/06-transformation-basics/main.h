#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Shader/Shader.h>
#include <Texture/Texture.h>
#include <cmath>
#include <WindowManager/WindowManager.h>

using std::cout;
using std::endl;

namespace _06_TransformationBasics {
    void main();
    unsigned int generate_vertex_buffer_object(const float *vertices, size_t vertices_size);
    unsigned int generate_element_buffer_object(const unsigned int *indices, size_t indices_size);
}

void _06_TransformationBasics::main() {
    float width { 800 };
    float height { 600 };

    const float vertices[] = {
        /* Positions */    /* Colors */        /* Texture 2D */
         .5f,  .5f, .0f,   1.0f,  .0f,  .0f,   1.0f, 1.0f,
         .5f, -.5f, .0f,    .0f, 1.0f,  .0f,   1.0f,  .0f,
        -.5f, -.5f, .0f,    .0f,  .0f, 1.0f,    .0f,  .0f,
        -.5f,  .5f, .0f,   1.0f, 1.0f,  .0f,    .0f, 1.0f
    };

    const unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    WindowManager manager { width, height };
    manager.init();

    /* Putting data to the buffer for GPU */
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO { generate_vertex_buffer_object(vertices, sizeof(vertices)) };
    unsigned int EBO { generate_element_buffer_object(indices, sizeof(indices)) };

    /* Shader */
    Shader shader_program {
        "./shaders/06-transformation-basics/vertex.glsl",
        "./shaders/06-transformation-basics/fragment.glsl"
    };

    /* Texture */
    // We abstracted out most of the texture into the custom Texture class
    Texture texture_1 { "./resources/textures/container.jpg", GL_RGB, GL_TEXTURE0 };
    try { texture_1.load(); }
    catch (TextureFailedToLoadException ex) { cout << ex.what() << endl; }

    Texture texture_2 { "./resources/textures/awesomeface.png", GL_RGBA, GL_TEXTURE1 };
    try { texture_2.load(); }
    catch (TextureFailedToLoadException ex) { cout << ex.what() << endl; }

    // We need to assign texture to the shader uniform via texture unit
    shader_program.use();
    shader_program.set_int("u_texture_1", 0);
    shader_program.set_int("u_texture_2", 1);

    float* opacity = new float { .5f };
    manager.listen(GLFW_KEY_UP, [opacity] (WindowManager* context) {
        *opacity += .01;
        if (*opacity > 1.) *opacity = 1.;
    });
    manager.listen(GLFW_KEY_DOWN, [opacity] (WindowManager* context) {
        *opacity -= .01;
        if (*opacity < .0) *opacity = .0;
    });

    /* Custom Transform */
    glm::mat4 transform_matrix { 1.f };
    glm::vec3 translation_vector { .5f, -.5f, .0f };
    transform_matrix = glm::translate(transform_matrix, translation_vector);
    transform_matrix = glm::rotate(transform_matrix, glm::radians(90.f), glm::vec3(.0, .0f, 1.0f));
    transform_matrix = glm::scale(transform_matrix, glm::vec3(.5f, .5f, 1.0f));
    
    manager.render([
        EBO,
        VAO,
        shader_program,
        texture_1,
        texture_2,
        opacity,
        transform_matrix
    ] (WindowManager* context) {
        glClearColor(.2f, .3f, .3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        texture_1.activate();
        texture_2.activate();

        shader_program.use();
        shader_program.set_float("opacity", *opacity);

        // Setting matrix 4D in shader
        GLint location { glGetUniformLocation(shader_program.ID, "transform") };
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(transform_matrix));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    });

    delete opacity;
    glfwTerminate();
}

unsigned int _06_TransformationBasics::generate_vertex_buffer_object(
    const float *vertices,
    size_t vertices_size
) {
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);

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

    return VBO;
}

unsigned int _06_TransformationBasics::generate_element_buffer_object(
    const unsigned int *indices,
    size_t indices_size
) {
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW);
    return EBO;
}
