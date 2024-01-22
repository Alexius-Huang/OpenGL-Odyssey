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

    void scale(float x, float y = 1.0f, float z = 1.0f) {
        this->matrix = glm::scale(this->matrix, glm::vec3(x, y, z));
    }

    void scale(glm::vec3 vector) {
        this->matrix = glm::scale(this->matrix, vector);
    }

    void rotate(float degree, float x, float y, float z) {
        this->matrix = glm::rotate(this->matrix, glm::radians(degree), glm::vec3(x, y, z));
    }

    void reset() {
        this->matrix = glm::mat4(1.0f);
    }

    glm::mat4 get_matrix() const { return this->matrix; }

    static Transform perspective_projection(float fov, float near, float far, float aspect_ratio) {
        Transform projection;
        projection.matrix = glm::perspective(glm::radians(fov), aspect_ratio, near, far);
        return projection;
    }

    static Transform look_at(
        float position_x, float position_y, float position_z,
        float target_x, float target_y, float target_z,
        float up_direction_x, float up_direction_y, float up_direction_z
    ) {
        Transform look_at;
        look_at.matrix = glm::lookAt(
            glm::vec3(position_x, position_y, position_z),
            glm::vec3(target_x, target_y, target_z),
            glm::vec3(up_direction_x, up_direction_y, up_direction_z)
        );
        return look_at;
    }

    static Transform look_at(
        glm::vec3 position,
        glm::vec3 target,
        glm::vec3 world_up_direction
    ) {
        return Transform::look_at(
            position.x, position.y, position.z,
            target.x, target.y, target.z,
            world_up_direction.x, world_up_direction.y, world_up_direction.z
        );
    }
};

#endif
