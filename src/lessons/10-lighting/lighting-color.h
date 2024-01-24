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
#include <Geometry/CubeV2.h>

using std::cout;
using std::endl;

namespace _10_1_LightingColor {
    void main();
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

void _10_1_LightingColor::main() {
    manager.init();
    manager.setup_camera_events(mouse_callback, scroll_callback);
    camera->set_position_z(5.0f);

    glm::vec3 light_color { 1.0f, 1.0f, 1.0f };
    glm::vec3 object_color { 1.0f, .5f, .31f };

    Geometry::CubeV2 light_source {};
    glm::vec3 light_source_position { 1.2f, 1.0f, 2.0f };
    Transform light_source_model {};
    light_source_model.translate(light_source_position);
    light_source_model.scale(.2f, .2f, .2f);
    Shader light_source_shader {
        "./shaders/Geometry/Cube/Cube.vs",
        "./shaders/Geometry/Cube/Cube.fs"
    };
    light_source_shader.use();
    light_source_shader.set_vec3("u_color", light_color);

    Geometry::CubeV2 obj {};
    Shader obj_shader {
        "./shaders/10-lighting/lighting-color.vs",
        "./shaders/10-lighting/lighting-color.fs"
    };
    obj_shader.use();
    obj_shader.set_vec3("u_color", object_color);
    obj_shader.set_vec3("u_light_color", light_color);
    glm::vec3 obj_position { .0f, .0f, .0f };
    Transform obj_model {};
    obj_model.translate(obj_position);

    /* Render Loop */
    glEnable(GL_DEPTH_TEST);
    manager.render([
        light_source,
        light_source_shader,
        light_source_model,
        obj,
        obj_shader,
        obj_model
    ] (WindowManager* context) {
        glClearColor(.1f, .1f, .1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view { context->get_view_matrix() };
        glm::mat4 projection { context->get_projection_matrix() };

        light_source_shader.use();
        light_source_shader.set_coordinates(light_source_model.get_matrix(), view, projection);
        light_source.render();

        obj_shader.use();
        obj_shader.set_coordinates(obj_model.get_matrix(), view, projection);
        obj.render();
    });

    /* Clean Up */
    delete fov;
    glfwTerminate();
}

void _10_1_LightingColor::mouse_callback(GLFWwindow* window, double x, double y) {
    camera->process_cursor(x, y);
}

void _10_1_LightingColor::scroll_callback(GLFWwindow* window, double x_offset, double y_offset) {
    *fov -= y_offset;
    if (*fov < ZOOM_MIN) *fov = ZOOM_MIN;
    else if (*fov > ZOOM_MAX) *fov = ZOOM_MAX;

    manager.set_fov(*fov);
}
