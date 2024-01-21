#ifndef _WINDOW_MANAGER_H_
#define _WINDOW_MANAGER_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <exception>
#include <map>
#include <functional>
#include <iostream>

using std::exception;
using std::string;
using std::map;
using std::function;
using std::pair;

class WindowManager;

typedef map<unsigned int, function<void (WindowManager *context)>> Events;
typedef Events::iterator EventsIter;

class WindowFailedToInitException : public exception {
private:
    string reason;
public:
    WindowFailedToInitException(string reason): reason { reason } {}

    const char* what() const noexcept override {
        string msg { "Failed to create GLFW window" };
        msg += " : " + this->reason;
        const char* msg_str { msg.c_str() };
        return msg_str;
    }
};

class WindowManager {
private:
    float width;
    float height;
    GLFWwindow* window;

    float delta_time { .0f };
    float last_frame_time { .0f };

    Events events;

    void execute_events() {
        for (
            EventsIter it { this->events.begin() };
            it != this->events.end();
            ++it
        ) {
            if (glfwGetKey(this->window, it->first) == GLFW_PRESS)
                it->second(this);
        }
    }

public:
    WindowManager(float width, float height)
        : width { width }, height { height } {}

    GLFWwindow* init() {
        glfwInit();

        // Setup OpenGL version
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Compatibility with Mac
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        this->window = glfwCreateWindow(width, height, "Learn OpenGL", NULL, NULL);

        if (this->window == NULL) {
            throw WindowFailedToInitException("Unable to create GLFW window");
        }

        glfwMakeContextCurrent(this->window);
        glfwSetFramebufferSizeCallback(this->window, WindowManager::framebuffer_size_callback);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            throw WindowFailedToInitException("Unable to initialize GLAD");
        }

        // Listens to escape events which closes the window
        this->listen(GLFW_KEY_ESCAPE, [] (WindowManager* context) {
            glfwSetWindowShouldClose(context->get_window(), true);
        });

        return this->window;
    }

    // Handle window resize
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    template <typename Lambda>
    void listen(unsigned int key, Lambda callback) {
        this->events.insert(pair(key, callback));
    }

    template <typename Lambda>
    void render(Lambda callback) {
        while (!glfwWindowShouldClose(this->window)) {
            GLFWwindow* window { this->window };

            float current_time { (float)glfwGetTime() };
            this->delta_time = current_time - this->last_frame_time;

            this->execute_events();

            callback(this);

            this->last_frame_time = current_time;

            glfwSwapBuffers(this->window);
            glfwPollEvents();
        }
    }

    GLFWwindow* get_window() { return this->window; }
    float get_delta_time() { return this->delta_time; }
};

#endif
