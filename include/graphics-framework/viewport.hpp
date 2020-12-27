#ifndef VIEWPORT_HPP
#define VIEWPORT_HPP

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

    #include "shader.hpp"
    #include "geometry.hpp"
    #include "renderable.hpp"
    #include "viewport_grid.hpp"
    #include "arcball.hpp"
    #include "input_handler.hpp"
    #include "camera.hpp"

    class Viewport: public std::enable_shared_from_this<Viewport> {
        public:
            Viewport(GLFWwindow *window, glm::vec3 background_colour);
            ~Viewport();

            std::shared_ptr<Viewport> GetSharedPtr() {
                return shared_from_this();
            }

            void Update(double deltaT);  
            void SetupTransformShader(GLuint transformShader);   
            
            void WindowSizeCallback(GLFWwindow* window, int width, int height);
            void SetFpsCounter(GLFWwindow* window, double deltaT);
            void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
            void MouseButtonCallback( GLFWwindow* window, int button, int action, int mods );
            void CursorCallback( GLFWwindow* window, double x, double y );
            void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

            //TODO: most of these can be made private.
            GLFWwindow *glfw_window;
            std::vector<std::pair<std::shared_ptr<Geometry>, std::shared_ptr<Renderable>>> geo_renderable_pairs;

            GLuint t_shader;      
            GLuint shader_id;    
            GLuint basic_shader;

            int width = -1, height = -1;

            double time_elapsed = 0;
            int frames_elapsed = 0;

            std::vector<InputHandler*> input_handlers;
            Arcball* arcball_camera;
            Camera* camera;
    };

#endif