#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Transform/Transform.h>
#include <WindowManager/WindowManager.h>

class Camera {
    friend class WindowManager;
private:
    float screen_width;
    float screen_height;
    float pitch;
    float yaw;
    float sensitivity;
    float previous_x;
    float previous_y;
    glm::vec3 position;
    glm::vec3 world_up_direction;

    inline glm::vec3 derive_front() {
        return glm::normalize(glm::vec3 {
            cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch)),
            sin(glm::radians(this->pitch)),
            sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch))
        });
    }

public:
    Camera(
        float screen_width,
        float screen_height,
        glm::vec3 position = glm::vec3 { .0f, .0f, 3.0f },
        glm::vec3 world_up_direction = glm::vec3 { .0f, 1.0f, .0f }
    ) : screen_width { screen_width },
        screen_height { screen_height },
        previous_x { screen_width / 2 },
        previous_y { screen_height / 2 },
        pitch { .0f },
        yaw { -90.0f },
        position { position },
        world_up_direction { world_up_direction },
        sensitivity { .1f }
    {}

    inline Transform derive_view_matrix() {
        return Transform::look_at(
            this->position,
            this->position + this->derive_front(),
            this->world_up_direction
        );
    }

    inline void set_position(glm::vec3 position) {
        this->position = position;
    }
    inline void set_position(float x, float y = .0f, float z = .0f) {
        this->position.x = x;
        this->position.y = y;
        this->position.z = z;
    }
    inline void set_position_x(float x) {
        this->position.x = x;
    }
    inline void set_position_y(float y) {
        this->position.y = y;
    }
    inline void set_position_z(float z) {
        this->position.z = z;
    }

    glm::vec3 get_position() { return this->position; }
    glm::vec3 get_front_vector() { return this->derive_front(); }

    void process_cursor(double x, double y) {
        float x_offset = x - this->previous_x;
        float y_offset = -(y - this->previous_y); // y is from bottom to top!

        this->previous_x = x;
        this->previous_y = y;

        x_offset *= sensitivity;
        y_offset *= sensitivity;

        this->yaw += x_offset;
        this->pitch += y_offset;

        if (this->pitch > 90.0f) {
            this->pitch = 90.0f;
        } else if (this->pitch < -90.0f) {
            this->pitch = -90.0f;
        }
    }
};

#endif
