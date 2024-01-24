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
#include <vector>

using std::cout;
using std::endl;
using std::vector;

namespace _11_3_SimulatingMaterial {
    void main();
    void mouse_callback(GLFWwindow* window, double x, double y);
    void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);

    vector<float> calc_vertex_normals(float vertices[], float vertices_count) {
        vector<float> result {};
        for (size_t i { 0 }; i < vertices_count / 9; i++) {
            glm::vec3 v1 { vertices[i * 9 + 0], vertices[i * 9 + 1], vertices[i * 9 + 2] };
            glm::vec3 v2 { vertices[i * 9 + 3], vertices[i * 9 + 4], vertices[i * 9 + 5] };
            glm::vec3 v3 { vertices[i * 9 + 6], vertices[i * 9 + 7], vertices[i * 9 + 8] };

            glm::vec3 v1_normal { glm::normalize(glm::cross(v2 - v1, v3 - v1)) };
            glm::vec3 v2_normal { glm::normalize(glm::cross(v3 - v2, v1 - v2)) };
            glm::vec3 v3_normal { glm::normalize(glm::cross(v1 - v3, v2 - v3)) };

            result.push_back(v1_normal.x);
            result.push_back(v1_normal.y);
            result.push_back(v1_normal.z);
            result.push_back(v2_normal.x);
            result.push_back(v2_normal.y);
            result.push_back(v2_normal.z);
            result.push_back(v3_normal.x);
            result.push_back(v3_normal.y);
            result.push_back(v3_normal.z);
        }

        return result;
    }

    float width { 800 };
    float height { 600 };

    // We change the value of fov in order to simulate zoom effect
    float *fov = new float { 45.0f };
    const float ZOOM_MIN { 1.0f };
    const float ZOOM_MAX { 75.0f };

    WindowManager manager { width, height };
    Camera* camera { manager.get_camera() };
}

void _11_3_SimulatingMaterial::main() {
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
        "./shaders/11-materials/light-property.vs",
        "./shaders/11-materials/light-property.fs"
    };
    obj_shader.use();
    obj_shader.set_vec3("u_color", object_color);

    /* Calculate Cube Normal and Put Data into Another VBO */
    vector<float> vertex_normals { calc_vertex_normals(Geometry::CUBE_VERTICES, Geometry::CUBE_VERTICES_COUNT) };
    unsigned int VBO_normal {};
    glGenBuffers(1, &VBO_normal);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normal);
    glBufferData(GL_ARRAY_BUFFER, vertex_normals.size() * sizeof(float), vertex_normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(1);

    /* Setting material properties */
    // Simulating emerald
    obj_shader.set_vec3("u_material.ambient", .0215f, .1745f, .0215f);
    obj_shader.set_vec3("u_material.diffuse", .07568f, .61424f, .07568f);
    obj_shader.set_vec3("u_material.specular", .633f, .727811f, .633f);
    obj_shader.set_float("u_material.shininess", 32.f);

    /* Setting light property */
    obj_shader.set_vec3("u_light.position", light_source_position);

    obj_shader.set_vec3("u_light.ambient", 1.0f, 1.0f, 1.0f);
    obj_shader.set_vec3("u_light.diffuse", 1.0f, 1.0f, 1.0f);
    obj_shader.set_vec3("u_light.specular", 1.0f, 1.0f, 1.0f);

    float* light_color_r = new float { .0f };
    float* light_color_g = new float { .0f };
    float* light_color_b = new float { .0f };

    glm::vec3 obj_position { .0f, .0f, .0f };
    Transform *obj_model = new Transform {};
    obj_model->translate(obj_position);

    Transform obj_normal { *obj_model };
    obj_normal.inverse();
    obj_normal.transpose();

    /* Render Loop */
    glEnable(GL_DEPTH_TEST);
    manager.render([
        light_source,
        light_source_shader,
        light_source_model,
        light_color_r,
        light_color_g,
        light_color_b,
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
        light_source_shader.set_vec3("u_color", glm::vec3(1.0f));
        light_source.render();

        obj_model->reset();
        // obj_model->translate(cos(glm::radians(context->get_time() * 30.0f)), .0f, .0f);
        // obj_model->rotate(context->get_time() * 30.0f, 1.0f, .0f, .0f);

        obj_shader.use();
        obj_shader.set_coordinates(obj_model->get_matrix(), view, projection);
        obj_shader.set_vec3("u_camera_position", context->get_camera()->get_position());

        obj.render();
    });

    /* Clean Up */
    delete fov;
    delete light_color_r;
    delete light_color_g;
    delete light_color_b;
    glfwTerminate();
}

void _11_3_SimulatingMaterial::mouse_callback(GLFWwindow* window, double x, double y) {
    camera->process_cursor(x, y);
}

void _11_3_SimulatingMaterial::scroll_callback(GLFWwindow* window, double x_offset, double y_offset) {
    *fov -= y_offset;
    if (*fov < ZOOM_MIN) *fov = ZOOM_MIN;
    else if (*fov > ZOOM_MAX) *fov = ZOOM_MAX;

    manager.set_fov(*fov);
}
