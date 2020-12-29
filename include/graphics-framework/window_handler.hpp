#ifndef WINDOW_HANDLER_HPP
#define WINDOW_HANDLER_HPP

    #include <array>
    #include <iostream>
    #include <cstdio>
    #include <cstdlib>
    #include <vector>
    #include <memory>
    #include <map>

    #define GLM_ENABLE_EXPERIMENTAL
    #include <GL/glew.h>
    #include <GL/glxmd.h>
    #include <GLFW/glfw3.h>
    #include <glm/glm.hpp>
    #include <glm/gtc/matrix_transform.hpp>
    #include <glm/gtc/type_ptr.hpp>
    #include <glm/gtx/transform.hpp>
    #include <glm/gtx/string_cast.hpp>

    #include "viewport.hpp"

    class WindowHandler: public std::enable_shared_from_this<WindowHandler> {
        public:
            explicit WindowHandler(GLFWwindow *window);
            ~WindowHandler();

            std::shared_ptr<WindowHandler> GetSharedPtr() {
                return shared_from_this();
            }

            void Update(double deltaT);  
            void SetFpsCounter(GLFWwindow* window, double deltaT);
            
            void WindowSizeCallback(GLFWwindow* window, int width, int height);
            void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
            void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
            void CursorCallback( GLFWwindow* window, double x, double y );
            void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

            void SetupTransformShader(GLuint transformShader);

            static const int VIEWPORT_SLOTS_X = 2, VIEWPORT_SLOTS_Y = 2;

            GLuint t_shader;      
            GLuint shader_id;    
            GLuint basic_shader;
            double time_elapsed = 0;
            int frames_elapsed = 0;

            std::vector<std::shared_ptr<Viewport>> viewports;
            
            //TODO: make 1D with an (x, y) accessor
            // std::array<std::array<std::shared_ptr<Viewport>, VIEWPORT_SLOTS_Y>, VIEWPORT_SLOTS_X>(viewport_slots);  

            int width = -1, height = -1;
            double cursor_x, cursor_y;
            GLFWwindow *glfw_window;
        private:
            std::shared_ptr<Viewport> focused_viewport_;

    };

#endif
