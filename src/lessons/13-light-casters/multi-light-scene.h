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

namespace _13_5_MultiLightScene {
    void main();
    void mouse_callback(GLFWwindow* window, double x, double y);
    void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);

    vector<float> calc_vertex_normals(float vertices[], float vertices_count);

    float width { 800 };
    float height { 600 };

    // We change the value of fov in order to simulate zoom effect
    float *fov = new float { 45.0f };
    const float ZOOM_MIN { 1.0f };
    const float ZOOM_MAX { 75.0f };

    WindowManager manager { width, height };
    Camera* camera { manager.get_camera() };
}

void _13_5_MultiLightScene::main() {
    manager.init();
    manager.setup_camera_events(mouse_callback, scroll_callback);
    camera->set_position_z(5.0f);

    glm::vec3 point_light_positions[] = {
       glm::vec3( 0.7f,  0.2f,   2.0f),
       glm::vec3( 2.3f, -3.3f,  -4.0f),
       glm::vec3(-4.0f,  2.0f, -8.0f),
       glm::vec3( 0.0f,  0.0f,  -3.0f)
    };

    glm::vec3 point_light_colors[] = {
       glm::vec3(1.0f, .0f, .0f), 
       glm::vec3(.0f, 1.0f, .0f), 
       glm::vec3(.0f, .0f, 1.0f), 
       glm::vec3(1.0f, 1.0f, 1.0f), 
    };

    Geometry::CubeV2 point_light {};
    Shader point_light_shader {
        "./shaders/Geometry/Cube/Cube.vs",
        "./shaders/Geometry/Cube/Cube.fs"
    };

    Geometry::CubeV2 obj {};
    Shader obj_shader {
        "./shaders/13-light-casters/multi-light-scene.vs",
        "./shaders/13-light-casters/multi-light-scene.fs"
    };
    obj_shader.use();

    glm::vec3 obj_positions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    /* Calculate Cube Normal and Put Data into Another VBO */
    vector<float> vertex_normals { calc_vertex_normals(Geometry::CUBE_VERTICES, Geometry::CUBE_VERTICES_COUNT) };
    unsigned int VBO_normal {};
    glGenBuffers(1, &VBO_normal);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normal);
    glBufferData(GL_ARRAY_BUFFER, vertex_normals.size() * sizeof(float), vertex_normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(1);

    /* Loading the texture coord */
    unsigned int VBO_texture_coord {};
    glGenBuffers(1, &VBO_texture_coord);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_texture_coord);
    glBufferData(GL_ARRAY_BUFFER, Geometry::CUBE_TEXTURE_COORD_SIZE, Geometry::CUBE_TEXTURE_COORDS, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(2);

    /* Setting material properties */
    // Loading diffuse map and set into diffuse
    Texture diffuse_map { "./resources/textures/container2.png", GL_RGBA, GL_TEXTURE0 };
    try { diffuse_map.load(); }
    catch (TextureFailedToLoadException ex) { cout << ex.what() << endl; }

    // Loading specular map and set into specular
    Texture specular_map { "./resources/textures/container2_specular.png", GL_RGBA, GL_TEXTURE1 };
    try { specular_map.load(); }
    catch (TextureFailedToLoadException ex) { cout << ex.what() << endl; }

    obj_shader.set_int("u_material.diffuse", 0);
    obj_shader.set_int("u_material.specular", 1);
    obj_shader.set_float("u_material.shininess", 64.f);

    /* Setting directional light property */
    obj_shader.set_vec3("u_directional_light.direction", -.2f, -1.0f, -.3f);
    obj_shader.set_vec3("u_directional_light.ambient", .1f, .0f, .1f);
    obj_shader.set_vec3("u_directional_light.diffuse", .25f, .0f, .25f);
    obj_shader.set_vec3("u_directional_light.specular", .75f, .0f, .75f);

    /* Setting Point light property */
    obj_shader.set_vec3("u_point_lights[0].position", point_light_positions[0]);
    obj_shader.set_vec3("u_point_lights[1].position", point_light_positions[1]);
    obj_shader.set_vec3("u_point_lights[2].position", point_light_positions[2]);
    obj_shader.set_vec3("u_point_lights[3].position", point_light_positions[3]);

    obj_shader.set_vec3("u_point_lights[0].ambient", .2f, .0f, .0f);
    obj_shader.set_vec3("u_point_lights[0].diffuse", .5f, .0f, .0f);
    obj_shader.set_vec3("u_point_lights[0].specular", 1.0f, .0f, .0f);
    obj_shader.set_vec3("u_point_lights[1].ambient", .0f, .2f, .0f);
    obj_shader.set_vec3("u_point_lights[1].diffuse", .0f, .5f, .0f);
    obj_shader.set_vec3("u_point_lights[1].specular", .0f, 1.0f, .0f);
    obj_shader.set_vec3("u_point_lights[2].ambient", .0f, .0f, .2f);
    obj_shader.set_vec3("u_point_lights[2].diffuse", .0f, .0f, .5f);
    obj_shader.set_vec3("u_point_lights[2].specular", .0f, .0f, 1.0f);
    obj_shader.set_vec3("u_point_lights[3].ambient", .2f, .2f, .2f);
    obj_shader.set_vec3("u_point_lights[3].diffuse", .5f, .5f, .5f);
    obj_shader.set_vec3("u_point_lights[3].specular", 1.0f, 1.0f, 1.0f);

    obj_shader.set_float("u_point_lights[0].attenuation.constant", 1.0f);
    obj_shader.set_float("u_point_lights[0].attenuation.linear", .09f);
    obj_shader.set_float("u_point_lights[0].attenuation.quadratic", .032f);
    obj_shader.set_float("u_point_lights[1].attenuation.constant", 1.0f);
    obj_shader.set_float("u_point_lights[1].attenuation.linear", .09f);
    obj_shader.set_float("u_point_lights[1].attenuation.quadratic", .032f);
    obj_shader.set_float("u_point_lights[2].attenuation.constant", 1.0f);
    obj_shader.set_float("u_point_lights[2].attenuation.linear", .09f);
    obj_shader.set_float("u_point_lights[2].attenuation.quadratic", .032f);
    obj_shader.set_float("u_point_lights[3].attenuation.constant", 1.0f);
    obj_shader.set_float("u_point_lights[3].attenuation.linear", .09f);
    obj_shader.set_float("u_point_lights[3].attenuation.quadratic", .032f);

    /* Render Loop */
    glEnable(GL_DEPTH_TEST);
    manager.render([
        point_light,
        point_light_shader,
        point_light_positions,
        point_light_colors,
        obj,
        obj_shader,
        obj_positions,
        diffuse_map,
        specular_map
    ] (WindowManager* context) {
        glClearColor(.1f, .1f, .1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view { context->get_view_matrix() };
        glm::mat4 projection { context->get_projection_matrix() };

        point_light_shader.use();
        for (size_t i { 0 }; i < 4; i++) {
            Transform model {};
            model.translate(point_light_positions[i]);
            model.scale(.2f, .2f, .2f);
            point_light_shader.set_vec3("u_color", point_light_colors[i]);
            point_light_shader.set_coordinates(model, view, projection);
            point_light.render();
        }

        diffuse_map.activate();
        specular_map.activate();

        obj_shader.use();
        obj_shader.set_coordinates(glm::mat4(.1f), view, projection);
        obj_shader.set_vec3("u_camera_position", context->get_camera()->get_position());

        for (size_t i { 0 }; i < 10; i++) {
            glm::vec3 obj_position { obj_positions[i] };
            Transform model {};
            model.translate(obj_position);

            float angle = 20.0f * i + context->get_time() * 30.0f;
            model.rotate(angle, 1.0f, .3f, .5f);

            obj_shader.set_mat4("model", model.get_matrix());

            Transform obj_normal { model.get_matrix() };
            obj_normal.inverse();
            obj_normal.transpose();
            obj_shader.set_mat3("normal_transform", obj_normal.get_matrix());

            obj.render();
        }
    });

    /* Clean Up */
    delete fov;
    glfwTerminate();
}

void _13_5_MultiLightScene::mouse_callback(GLFWwindow* window, double x, double y) {
    camera->process_cursor(x, y);
}

void _13_5_MultiLightScene::scroll_callback(GLFWwindow* window, double x_offset, double y_offset) {
    *fov -= y_offset;
    if (*fov < ZOOM_MIN) *fov = ZOOM_MIN;
    else if (*fov > ZOOM_MAX) *fov = ZOOM_MAX;

    manager.set_fov(*fov);
}

vector<float> _13_5_MultiLightScene::calc_vertex_normals(float vertices[], float vertices_count) {
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
