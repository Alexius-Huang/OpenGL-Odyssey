#include "Shader.h"

using std::string;
using std::ifstream;
using std::stringstream;
using std::cout;
using std::endl;

Shader Shader::from_source(const char* vertex_shader_code, const char* fragment_shader_code) {
    // Compiling shaders...
    int success;
    char info_log[512];

    unsigned int vertex_shader { glCreateShader(GL_VERTEX_SHADER) };
    unsigned int fragment_shader { glCreateShader(GL_FRAGMENT_SHADER) };

    // cout << "Compiling Vertex Shader ... " << endl;
    // cout << vertex_shader_code << endl;
    glShaderSource(vertex_shader, 1, &vertex_shader_code, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_ERROR:" << endl << info_log << endl;
    }

    // cout << "Compiling Fragment Shader ... " << endl;
    // cout << fragment_shader_code << endl;
    glShaderSource(fragment_shader, 1, &fragment_shader_code, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_ERROR:" << endl << info_log << endl;
    }

    return Shader { vertex_shader, fragment_shader };
};

Shader::Shader(unsigned int vertex_shader, unsigned int fragment_shader) {
    this->ID = glCreateProgram();
    glAttachShader(this->ID, vertex_shader);
    glAttachShader(this->ID, fragment_shader);
    glLinkProgram(this->ID);
    
    int success;
    char info_log[512];

    glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->ID, 512, NULL, info_log);
        cout << "ERROR::SHADER::PROGRAM::LINK_FAILED:" << endl << info_log << endl;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

Shader::Shader(const char* vertex_shader_path, const char* fragment_shader_path) {
    string vertex_shader_string;
    string fragment_shader_string;
    ifstream vertex_shader_file, fragment_shader_file;

    vertex_shader_file.exceptions(ifstream::failbit | ifstream::badbit);
    fragment_shader_file.exceptions(ifstream::failbit | ifstream::badbit);

    try {
        vertex_shader_file.open(vertex_shader_path);
        fragment_shader_file.open(fragment_shader_path);

        stringstream vertex_shader_stream, fragment_shader_stream;
        vertex_shader_stream << vertex_shader_file.rdbuf();
        fragment_shader_stream << fragment_shader_file.rdbuf();

        vertex_shader_file.close();
        fragment_shader_file.close();

        vertex_shader_string = vertex_shader_stream.str();
        fragment_shader_string = fragment_shader_stream.str();
    } catch (ifstream::failure e) {
        cout << "ERROR::SHADER::FILE_NOT_READ_SUCCESSFULLY:" << endl << e.what() << endl;
    }

    const char* vertex_shader_code { vertex_shader_string.c_str() };
    const char* fragment_shader_code { fragment_shader_string.c_str() };

    // Compiling shaders...
    int success;
    char info_log[512];

    unsigned int vertex_shader { glCreateShader(GL_VERTEX_SHADER) };
    unsigned int fragment_shader { glCreateShader(GL_FRAGMENT_SHADER) };

    // cout << "Compiling Vertex Shader ... " << endl;
    // cout << vertex_shader_code << endl;
    glShaderSource(vertex_shader, 1, &vertex_shader_code, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_ERROR:" << endl << info_log << endl;
    }

    // cout << "Compiling Fragment Shader ... " << endl;
    // cout << fragment_shader_code << endl;
    glShaderSource(fragment_shader, 1, &fragment_shader_code, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_ERROR:" << endl << info_log << endl;
    }

    this->ID = glCreateProgram();
    glAttachShader(this->ID, vertex_shader);
    glAttachShader(this->ID, fragment_shader);
    glLinkProgram(this->ID);

    glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->ID, 512, NULL, info_log);
        cout << "ERROR::SHADER::PROGRAM::LINK_FAILED:" << endl << info_log << endl;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void Shader::use() const {
    glUseProgram(this->ID);
}

void Shader::set_bool(const string name, bool value) const {
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int) value);
}

void Shader::set_int(const string name, int value) const {
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::set_float(const string name, float value) const {
    glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::set_vec3(const string name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(this->ID, name.c_str()), x, y, z);
}

void Shader::set_vec3(const string name, glm::vec3 vector) const {
    glUniform3f(glGetUniformLocation(this->ID, name.c_str()), vector.x, vector.y, vector.z);
}

void Shader::set_mat4(const string name, glm::mat4 matrix) const {
    glUniformMatrix4fv(
        glGetUniformLocation(this->ID, name.c_str()),
        1,
        GL_FALSE,
        glm::value_ptr(matrix)
    );
}

void Shader::set_coordinates(glm::mat4 model, glm::mat4 view, glm::mat4 projection) const {
    this->set_mat4("model", model);
    this->set_mat4("view", view);
    this->set_mat4("projection", projection);
}
void Shader::set_coordinates(Transform model, Transform view, Transform projection) const {
    this->set_mat4("model", model.get_matrix());
    this->set_mat4("view", view.get_matrix());
    this->set_mat4("projection", projection.get_matrix());
}
