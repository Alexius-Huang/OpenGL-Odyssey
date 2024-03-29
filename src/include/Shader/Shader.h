#ifndef _SHADER_H_
#define _SHADER_H_

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Transform/Transform.h>

using std::string;

class Shader {
public:
    static Shader from_source(const char* vertex_shader_code, const char* fragment_shader_code);
    unsigned int ID;

    Shader(unsigned int vertex_shader, unsigned int fragment_shader);
    Shader(const char* vertex_shader_path, const char* fragment_shader_path);

    // use/activate shader
    void use() const;

    // utility uniform functions
    void set_bool(const string name, bool value) const;
    void set_int(const string name, int value) const;
    void set_float(const string name, float value) const;
    void set_vec3(const string name, float x, float y, float z) const;
    void set_vec3(const string name, glm::vec3 vector) const;
    void set_mat3(const string name, glm::mat3 matrix) const;
    void set_mat4(const string name, glm::mat4 matrix) const;

    void set_coordinates(glm::mat4 model, glm::mat4 view, glm::mat4 projection) const;
    void set_coordinates(Transform model, Transform view, Transform projection) const;
};

#endif
