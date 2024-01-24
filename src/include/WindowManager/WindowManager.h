#ifndef _WINDOW_MANAGER_H_
#define _WINDOW_MANAGER_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <exception>
#include <map>
#include <functional>
#include <iostream>
#include <Camera/Camera.h>

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
    float fov;
    float near;
    float far;
    float aspect_ratio;
    GLFWwindow* window;
    Camera* camera;

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
    WindowManager(
        float width,
        float height,
        float fov = 45.0f,
        float near = .1f,
        float far = 100.0f
    ) : width { width },
        height { height },
        aspect_ratio { width / height },
        fov { fov },
        near { near },
        far { far },
        camera { nullptr }
    {
        this->camera = new Camera { width, height };
    }

    ~WindowManager() {
        delete this->camera;
    }

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

    void setup_camera_events(GLFWcursorposfun mouse_callback, GLFWscrollfun scroll_callback) {
        this->listen(GLFW_KEY_W, [] (WindowManager *context) {
            Camera* camera = context->get_camera();
            camera->set_position_y(camera->position.y + context->get_delta_time());
        });
        this->listen(GLFW_KEY_A, [] (WindowManager *context) {
            Camera* camera = context->get_camera();
            camera->set_position_x(camera->position.x - context->get_delta_time());
        });
        this->listen(GLFW_KEY_S, [] (WindowManager *context) {
            Camera* camera = context->get_camera();
            camera->set_position_y(camera->position.y - context->get_delta_time());
        });
        this->listen(GLFW_KEY_D, [] (WindowManager *context) {
            Camera* camera = context->get_camera();
            camera->set_position_x(camera->position.x + context->get_delta_time());
        });

        glfwSetCursorPosCallback(this->get_window(), mouse_callback);
        glfwSetScrollCallback(this->get_window(), scroll_callback);
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

            float current_time { this->get_time() };
            this->delta_time = current_time - this->last_frame_time;

            this->execute_events();

            callback(this);

            this->last_frame_time = current_time;

            glfwSwapBuffers(this->window);
            glfwPollEvents();
        }
    }

    GLFWwindow* get_window() const { return this->window; }
    float get_delta_time() const { return this->delta_time; }
    float get_time() const { return (float)glfwGetTime(); }
    Camera* get_camera() const { return this->camera; }

    glm::mat4 get_view_matrix() const {
        return this->camera->derive_view_matrix().get_matrix();
    }

    glm::mat4 get_projection_matrix() const {
        return glm::perspective(
            glm::radians(this->fov),
            this->aspect_ratio,
            this->near,
            this->far
        );
    }

    void set_fov(float fov) {
        this->fov = fov;
    }
};

#endif
