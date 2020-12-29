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

    #include "window_handler.hpp"
    #include "viewport.hpp"

    class InputRouter {
        public:
            explicit InputRouter(std::shared_ptr<WindowHandler> active_window_handler);
            InputRouter() = default;

            static std::weak_ptr<WindowHandler> GetActiveWindowHandler();
            static void SetActiveWindowHandler(std::shared_ptr<WindowHandler> window_handler);
            static void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
            static void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset);
            static void CursorCallback(GLFWwindow *window, double x, double y);
            static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
            static void WindowSizeCallback(GLFWwindow *window, int width, int height);

        protected:

    };

#endif
