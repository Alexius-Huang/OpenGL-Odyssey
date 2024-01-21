#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <WindowManager/WindowManager.h>

using std::ifstream;
using std::stringstream;
using std::string;
using std::cout;
using std::endl;

namespace _02_FirstTriangle {
    void main();

    string read_shader_file(const char* path);
}

void _02_FirstTriangle::main() {
    float width { 800 };
    float height { 600 };

    const float vertices[] {
        -.5f, -.5f, .0f,
         .5f, -.5f, .0f,
         .0f,  .5f, .0f
    };

    /* Creating Window */
    // To simplify the code, we now use the custom WindowManager
    // which already handles all the setups from the 01-introduction
    WindowManager manager { width, height };
    GLFWwindow* window { manager.init() };

    /* Vertex Array Object */
    // We use VBO to bind to the target array and draw with any vertices data
    // stored in VBO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    /* Vertex Buffer Object */
    // Storing vertices to a buffer and then let GPU to retrieve those data
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Apply the buffer data
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Setup the VBO array pointer which points to location = 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    /* Create Shaders */
    int success;
    char info_log[512];

    string vertex_shader_source = read_shader_file("./shaders/02-first-triangle/vertex.glsl");
    const char* vertex_shader_code { vertex_shader_source.c_str() };
    string fragment_shader_source = read_shader_file("./shaders/02-first-triangle/fragment.glsl");
    const char* fragment_shader_code { fragment_shader_source.c_str() };

    unsigned int vertex_shader { glCreateShader(GL_VERTEX_SHADER) };
    glShaderSource(vertex_shader, 1, &vertex_shader_code, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED: " << info_log << endl;
    }

    unsigned int fragment_shader { glCreateShader(GL_FRAGMENT_SHADER) };
    glShaderSource(fragment_shader, 1, &fragment_shader_code, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: " << info_log << endl;
    }

    /* Create Shader Program */
    unsigned int shader_program { glCreateProgram() };
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        cout << "ERROR::SHADER::PROGRAM::LINK_FAILED: " << info_log << endl;
    }

    // We don't need shaders anymore!
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    /* Render Loop */
    manager.render([
        shader_program,
        VAO
    ] (WindowManager* context) {
        glClearColor(.2f, .3f, .3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Draw Triangles */
        glUseProgram(shader_program);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 9);
    });

    /* Clean Up */
    glfwTerminate();
}

string _02_FirstTriangle::read_shader_file(const char* path) {
    string shader_string;
    ifstream shader_file;

    shader_file.exceptions(ifstream::failbit | ifstream::badbit);

    try {
        shader_file.open(path);
        stringstream shader_stream;
        shader_stream << shader_file.rdbuf();

        shader_file.close();

        shader_string = shader_stream.str();
    } catch (ifstream::failure e) {
        cout << "ERROR::SHADER::FILE_NOT_READ_SUCCESSFULLY:" << endl << e.what() << endl;
    }

    return shader_string;
}