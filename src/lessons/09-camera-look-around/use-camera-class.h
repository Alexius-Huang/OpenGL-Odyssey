#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Shader/Shader.h>
#include <Texture/Texture.h>
#include <Transform/Transform.h>
#include <WindowManager/WindowManager.h>
#include <Camera/Camera.h>

using std::cout;
using std::endl;

namespace _09_4_UseCameraClass {
    void main();
    unsigned int generate_vertex_buffer_object(const float *vertices, size_t vertices_size);
    void mouse_callback(GLFWwindow* window, double x, double y);
    void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);

    float width { 800 };
    float height { 600 };

    // We change the value of fov in order to simulate zoom effect
    float *fov = new float { 45.0f };
    const float ZOOM_MIN { 1.0f };
    const float ZOOM_MAX { 75.0f };

    WindowManager manager { width, height };
    Camera* camera { manager.get_camera() };
}

void _09_4_UseCameraClass::main() {
    manager.init();

    /* We have abstracted the Camera which would already builtin WindowManager */
    manager.setup_camera_events(mouse_callback, scroll_callback);

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

    glm::vec3 cube_positions[] = {
        glm::vec3( 0.0f,  0.0f,   0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f,  -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f,  -3.5f),
        glm::vec3(-1.7f,  3.0f,  -7.5f),
        glm::vec3( 1.3f, -2.0f,  -2.5f),
        glm::vec3( 1.5f,  2.0f,  -2.5f),
        glm::vec3( 1.5f,  0.2f,  -1.5f),
        glm::vec3(-1.3f,  1.0f,  -1.5f)
    };
    size_t cube_position_count { sizeof(cube_positions) / sizeof(cube_positions[0]) };

    /* Put data into buffer for the GPU  */
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    generate_vertex_buffer_object(vertices, vertices_size);

    /* Shader */
    Shader shader_program {
        "./shaders/09-camera-look-around/vertex.glsl",
        "./shaders/09-camera-look-around/fragment.glsl"
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

    /* Render Loop */
    glEnable(GL_DEPTH_TEST);
    manager.render([
        opacity,
        VAO,
        shader_program,
        texture_1,
        texture_2,
        vertices_size,
        cube_positions,
        cube_position_count
    ] (WindowManager* context) {
        glClearColor(.2f, .3f, .3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        texture_1.activate();
        texture_2.activate();

        shader_program.use();
        shader_program.set_float("opacity", *opacity);
        shader_program.set_mat4("view", context->get_view_matrix());
        shader_program.set_mat4("projection", context->get_projection_matrix());

        for (size_t i { 0 }; i < cube_position_count; i++) {
            glm::vec3 cube_position { cube_positions[i] };

            Transform model {};
            model.translate(cube_position);

            shader_program.set_mat4("model", model.get_matrix());
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, vertices_size);
        }
    });

    /* Clean Up */
    delete opacity;
    delete fov;
    glfwTerminate();
}

unsigned int _09_4_UseCameraClass::generate_vertex_buffer_object(
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

void _09_4_UseCameraClass::mouse_callback(GLFWwindow* window, double x, double y) {
    camera->process_cursor(x, y);
}

void _09_4_UseCameraClass::scroll_callback(GLFWwindow* window, double x_offset, double y_offset) {
    *fov -= y_offset;
    if (*fov < ZOOM_MIN) *fov = ZOOM_MIN;
    else if (*fov > ZOOM_MAX) *fov = ZOOM_MAX;

    manager.set_fov(*fov);
}
