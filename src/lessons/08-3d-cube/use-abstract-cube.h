#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <Shader/Shader.h>
#include <Texture/Texture.h>
#include <Transform/Transform.h>
#include <WindowManager/WindowManager.h>
#include <Geometry/Cube.h>

using std::cout;
using std::endl;
using std::vector;

namespace _08_3DCube_4_UseAbstractCube {
    void main();
    unsigned int generate_vertex_buffer_object(const float *vertices, size_t vertices_size);
}

void _08_3DCube_4_UseAbstractCube::main() {
    float width { 800 };
    float height { 600 };

    WindowManager manager { width, height };
    manager.init();
    
    /* Geometry::Cube Custom Class */
    // Abstracted out all of the VAO, VBO buffer data fill along with shader
    // Geometry::Cube cube {};
    vector<Geometry::Cube> cubes {
        Geometry::Cube {},
        Geometry::Cube {},
        Geometry::Cube {},
        Geometry::Cube {},
        Geometry::Cube {},
        Geometry::Cube {}
    };
    vector<glm::vec3> cube_colors {
        glm::vec3 { 1.0f, .5f, 1.0f },
        glm::vec3 { .5f, 1.0f, 1.0f },
        glm::vec3 { .5f, .0f, 1.0f },
        glm::vec3 { .75f, 1.0f, .0f },
        glm::vec3 { .0f, .75f, .5f },
        glm::vec3 { .5f, .5f, 1.0f }
    };
    vector<glm::vec3> cube_positions {
        glm::vec3 { .0f, .0f, -1.0f },
        glm::vec3 { 1.2f, 1.6f, -2.0f },
        glm::vec3 { -1.4f, 1.2f, -4.0f },
        glm::vec3 { -1.7f, -1.3f, -1.5f },
        glm::vec3 { -2.2f, 1.1f, -2.0f },
        glm::vec3 { 1.5f, -1.4f, -3.5f }
    };

    /* Coordinate System */
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
        view,
        projection,
        cubes,
        cube_colors,
        cube_positions
    ] (WindowManager* context) {
        glClearColor(.2f, .3f, .3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (size_t i { 0 }; i < cubes.size(); i++) {
            Transform model {};
            model.translate(cube_positions.at(i));
            model.rotate(
                context->get_time() * 30.0f,
                .4f, .5f, .1f
            );

            Geometry::Cube cube { cubes.at(i) };
            cube.set_color(cube_colors.at(i));
            cube.set_coordinate_system(
                model.get_matrix(),
                view->get_matrix(),
                projection.get_matrix()
            );
            cube.render();
        }
    });

    /* Clean Up */
    delete view;
    delete view_x;
    delete view_y;
    glfwTerminate();
}

unsigned int _08_3DCube_4_UseAbstractCube::generate_vertex_buffer_object(
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
}