#ifndef INPUTROUTER_HPP
#define INPUTROUTER_HPP

    #include <iostream>
    #include <memory>
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
    #include <glm/glm.hpp>
    #include <glm/gtc/type_ptr.hpp>
    #include <glm/gtc/matrix_transform.hpp>
    #include <glm/gtx/rotate_vector.hpp>
    #include <glm/gtc/matrix_inverse.hpp>

    #include "viewport.hpp"

    using namespace glm;
    using namespace std;

    class InputRouter {
        public:
            InputRouter(shared_ptr<Viewport> active_viewport);
            InputRouter() {};

            static shared_ptr<Viewport> GetActiveViewport();
            static void SetActiveViewport(shared_ptr<Viewport> active_viewport);
            static void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods);
            static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
            static void cursorCallback(GLFWwindow *window, double x, double y);
            static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
            static void windowSizeCallback(GLFWwindow* window, int width, int height);

        protected:

    };

#endif