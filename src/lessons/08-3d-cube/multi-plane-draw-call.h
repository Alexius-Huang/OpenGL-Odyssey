#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Shader/Shader.h>
#include <Texture/Texture.h>
#include <Transform/Transform.h>
#include <WindowManager/WindowManager.h>

using std::cout;
using std::endl;

namespace _08_3DCube_1_MultiPlaneDrawCall {
    void main();
    unsigned int generate_vertex_buffer_object(const float *vertices, size_t vertices_size);
    unsigned int generate_element_buffer_object(const unsigned int *indices, size_t indices_size);
}

void _08_3DCube_1_MultiPlaneDrawCall::main() {
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
        "./shaders/08-3d-cube/vertex-1.glsl",
        "./shaders/08-3d-cube/fragment-1.glsl"
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

    /* Coordinate System */
    // Local space -> World space
    Transform *model = new Transform {};

    // World space -> View / Camera space
    Transform *view = new Transform {};
    view->translate(.0f, .0f, -3.0f);

    // View space -> Clip space
    float fov { 45.0f }; // field of view
    float aspect_ratio { width / height };
    float near { .1f };
    float far { 100.0f };
    Transform projection { glm::perspective(glm::radians(fov), aspect_ratio, near, far) };

    float *view_x = new float { .0f };
    float *view_y = new float { .0f };
    manager.listen(GLFW_KEY_D, [view_x, view_y, view] (WindowManager* context) {
        *view_x -= context->get_delta_time();
        if (*view_x < -1.f) *view_x = -1.f;
        view->reset();
        view->translate(*view_x, *view_y, -3.f);
    });
    manager.listen(GLFW_KEY_A, [view_x, view_y, view] (WindowManager* context) {
        *view_x += context->get_delta_time();
        if (*view_x > 1.0f) *view_x = 1.0f;
        view->reset();
        view->translate(*view_x, *view_y, -3.f);
    });
    manager.listen(GLFW_KEY_W, [view_x, view_y, view] (WindowManager* context) {
        *view_y -= context->get_delta_time();
        if (*view_y < -1.f) *view_y = -1.f;
        view->reset();
        view->translate(*view_x, *view_y, -3.f);
    });
    manager.listen(GLFW_KEY_S, [view_x, view_y, view] (WindowManager* context) {
        *view_y += context->get_delta_time();
        if (*view_y > 1.0f) *view_y = 1.0f;
        view->reset();
        view->translate(*view_x, *view_y, -3.f);
    });

    glEnable(GL_DEPTH_TEST);

    manager.render([
        EBO,
        VAO,
        shader_program,
        texture_1,
        texture_2,
        opacity,
        model,
        view,
        projection
    ] (WindowManager* context) {
        glClearColor(.2f, .3f, .3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        texture_1.activate();
        texture_2.activate();

        shader_program.use();
        shader_program.set_float("opacity", *opacity);

        // Setting matrix 4D in shader
        shader_program.set_mat4("model", model->get_matrix());
        shader_program.set_mat4("view", view->get_matrix());
        shader_program.set_mat4("projection", projection.get_matrix());

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        Transform new_model { model->get_matrix() };
        new_model.translate(.0f, .0f, -1.f);
        shader_program.set_mat4("model", new_model.get_matrix());

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        Transform new_model_2 { model->get_matrix() };
        new_model_2.translate(-.5f, .0f, -.5f);
        new_model_2.rotate(-90.f, .0f, 1.f, .0f);
        shader_program.set_mat4("model", new_model_2.get_matrix());

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        Transform new_model_3 { model->get_matrix() };
        new_model_3.translate(.5f, .0f, -.5f);
        new_model_3.rotate(90.f, .0f, 1.f, .0f);
        shader_program.set_mat4("model", new_model_3.get_matrix());

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        Transform new_model_4 { model->get_matrix() };
        new_model_4.translate(.0f, -.5f, -.5f);
        new_model_4.rotate(90.f, 1.f, .0f, .0f);
        shader_program.set_mat4("model", new_model_4.get_matrix());

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        Transform new_model_5 { model->get_matrix() };
        new_model_5.translate(.0f, .5f, -.5f);
        new_model_5.rotate(-90.f, 1.f, .0f, .0f);
        shader_program.set_mat4("model", new_model_5.get_matrix());

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    });

    delete opacity;
    delete model;
    delete view;
    delete view_x;
    delete view_y;
    glfwTerminate();
}

unsigned int _08_3DCube_1_MultiPlaneDrawCall::generate_vertex_buffer_object(
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

unsigned int _08_3DCube_1_MultiPlaneDrawCall::generate_element_buffer_object(
    const unsigned int *indices,
    size_t indices_size
) {
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW);
    return EBO;
}