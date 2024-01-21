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

using std::string;

class Shader {
public:
    unsigned int ID;

    Shader(const char* vertex_shader_path, const char* fragment_shader_path);

    // use/activate shader
    void use() const;

    // utility uniform functions
    void set_bool(const string name, bool value) const;
    void set_int(const string name, int value) const;
    void set_float(const string name, float value) const;
    void set_vec3(const string name, float x, float y, float z) const;
    void set_vec3(const string name, glm::vec3 vector) const;
    void set_mat4(const string name, glm::mat4 matrix) const;
};

#endif
