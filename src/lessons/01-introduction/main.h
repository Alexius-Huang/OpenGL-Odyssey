#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using std::cout;
using std::endl;

namespace _01_Introduction {
    void main();
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
}

void _01_Introduction::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void _01_Introduction::main() {
    unsigned int width { 800 };
    unsigned int height { 600 };

    /* Window Initialization */
    glfwInit();

    // Setup OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Compatibility with Mac
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Learn OpenGL", NULL, NULL);

    if (window == NULL) {
        cout << "Failed to load GLFW window" << endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    // Able to resize the window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /* Setting Up GLAD */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return;
    }

    /* Animation Loop */
    while (!glfwWindowShouldClose(window)) {
        // When we press the ESCAPE key, we end the program
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        // We must clear the screen before each new draw
        glClearColor(.2f, .3f, .3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* We can draw anything here now! */

        /**
         *  Double Buffer:
         *  
         *  When an application draws in a single buffer the resulting image
         *  may display flickering issues.
         * 
         *  This is because the resulting output image is not drawn in an
         *  instant, but drawn pixel by pixel and usually from left to right
         *  and top to bottom.
         * 
         *  Because this image is not displayed at an instant to the user
         *  while still being rendered to, the result may contain artifacts.
         * 
         *  To circumvent these issues, windowing applications apply a
         *  double buffer for rendering. The front buffer contains the final
         *  output image that is shown at the screen, while all the rendering
         *  commands draw to the back buffer.
         * 
         *  As soon as all the rendering commands are finished we swap the
         *  back buffer to the front buffer so the image can be displayed
         *  without still being rendered to, removing all the aforementioned
         *  artifacts.
         *  
         */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}
