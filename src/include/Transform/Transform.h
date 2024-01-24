#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Transform {
private:
    glm::mat4 matrix;
public:
    Transform(): matrix { glm::mat4(1.0f) } {}
    Transform(glm::mat4 matrix): matrix { matrix } {}

    void translate(float x, float y = .0f, float z = .0f) {
        this->matrix = glm::translate(this->matrix, glm::vec3(x, y, z));
    }

    void translate(glm::vec3 vector) {
        this->matrix = glm::translate(this->matrix, vector);
    }

    void translate(glm::vec4 vector) {
        this->matrix = glm::translate(this->matrix, glm::vec3(vector.x, vector.y, vector.z));
    }

    void scale(float x, float y = 1.0f, float z = 1.0f) {
        this->matrix = glm::scale(this->matrix, glm::vec3(x, y, z));
    }

    void scale(glm::vec3 vector) {
        this->matrix = glm::scale(this->matrix, vector);
    }

    void transpose() {
        this->matrix = glm::transpose(this->matrix);
    }

    void rotate(float degree, float x, float y, float z) {
        this->matrix = glm::rotate(this->matrix, glm::radians(degree), glm::vec3(x, y, z));
    }

    void reset() {
        this->matrix = glm::mat4(1.0f);
    }

    glm::mat4 get_matrix() const { return this->matrix; }
    void set_matrix(glm::mat4 matrix) { this->matrix = matrix; }

    static Transform perspective_projection(float fov, float near, float far, float aspect_ratio) {
        Transform projection;
        projection.matrix = glm::perspective(glm::radians(fov), aspect_ratio, near, far);
        return projection;
    }

    static Transform look_at(
        glm::vec3 position,
        glm::vec3 target,
        glm::vec3 world_up_direction
    ) {
        glm::vec3 inward_direction { glm::normalize(position - target) };
        glm::vec3 right_direction { glm::normalize(glm::cross(world_up_direction, inward_direction)) };
        glm::vec3 up_direction { glm::cross(inward_direction, right_direction) };

        Transform look_at {
            glm::mat4(
                glm::vec4(right_direction, .0f),
                glm::vec4(up_direction, .0f),
                glm::vec4(inward_direction, .0f),
                glm::vec4(.0f, .0f, .0f, 1.0f)
            )
        };
        look_at.transpose();
        look_at.translate(-position);
        return look_at;

        /* Using glm::look_at produces the same result */
        // glm::lookAt(
        //     glm::vec3(position_x, position_y, position_z),
        //     glm::vec3(target_x, target_y, target_z),
        //     glm::vec3(up_direction_x, up_direction_y, up_direction_z)
        // )
    }

    static Transform look_at(
        float position_x, float position_y, float position_z,
        float target_x, float target_y, float target_z,
        float up_direction_x, float up_direction_y, float up_direction_z
    ) {
        return Transform::look_at(
            glm::vec3(position_x, position_y, position_z),
            glm::vec3(target_x, target_y, target_z),
            glm::vec3(up_direction_x, up_direction_y, up_direction_z)
        );
    }
};

#endif
