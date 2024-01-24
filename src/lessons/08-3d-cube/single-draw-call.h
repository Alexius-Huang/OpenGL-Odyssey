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

namespace _08_3DCube_2_SingleDrawCall {
    void main();
    unsigned int generate_vertex_buffer_object(const float *vertices, size_t vertices_size);
}

void _08_3DCube_2_SingleDrawCall::main() {
    float width { 800 };
    float height { 600 };

    float vertices[] = {
        /* Positions */       /* Texture Coord */
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    size_t vertices_size { sizeof(vertices) };

    WindowManager manager { width, height };
    manager.init();

    /* Put data into buffer for the GPU  */
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    generate_vertex_buffer_object(vertices, vertices_size);

    /* Shader */
    Shader shader_program {
        "./shaders/08-3d-cube/vertex-2.glsl",
        "./shaders/08-3d-cube/fragment-2.glsl"
    };

    /* Texture */
    Texture texture_1 { "./resources/textures/container.jpg", GL_RGB, GL_TEXTURE0 };
    try { texture_1.load(); }
    catch (TextureFailedToLoadException ex) { cout << ex.what() << endl; }

    Texture texture_2 { "./resources/textures/awesomeface.png", GL_RGBA, GL_TEXTURE1 };
    try { texture_2.load(); }
    catch (TextureFailedToLoadException ex) { cout << ex.what() << endl; }

    shader_program.use();
    shader_program.set_int("u_texture_1", 0);
    shader_program.set_int("u_texture_2", 1);

    /* Some event setup */
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
    Transform *model = new Transform;
    Transform *view = new Transform;
    view->translate(.0f, .0f, -3.0f);

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

    float fov { 45.0f }; // field of view
    float aspect_ratio { width / height };
    float near { .1f };
    float far { 100.0f };
    Transform projection { glm::perspective(glm::radians(fov), aspect_ratio, near, far) };

    /* Render Loop */
    glEnable(GL_DEPTH_TEST);
    manager.render([
        model,
        view,
        projection,
        opacity,
        VAO,
        shader_program,
        texture_1,
        texture_2,
        vertices_size
    ] (WindowManager* context) {
        glClearColor(.2f, .3f, .3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        texture_1.activate();
        texture_2.activate();

        model->rotate(
            context->get_delta_time() * 30.0f,
            .4f, .5f, .1f
        );

        shader_program.use();
        shader_program.set_float("opacity", *opacity);
        shader_program.set_mat4("model", model->get_matrix());
        shader_program.set_mat4("view", view->get_matrix());
        shader_program.set_mat4("projection", projection.get_matrix());

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertices_size);
    });

    /* Clean Up */
    delete opacity;
    delete model;
    delete view;
    delete view_x;
    delete view_y;
    glfwTerminate();
}

unsigned int _08_3DCube_2_SingleDrawCall::generate_vertex_buffer_object(
    const float *vertices,
    size_t vertices_size
) {
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);

    size_t stride { 5 * sizeof(float) };
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    return VBO;
}