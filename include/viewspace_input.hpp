#ifndef VIEWSPACEINPUT_HPP
#define VIEWSPACEINPUT_HPP

    // #include "stdafx.h"
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
    #include <glm/glm.hpp>
    #include <glm/gtc/matrix_transform.hpp>

    using namespace glm;

    class ViewspaceInput {
        public:
            ViewspaceInput(GLFWwindow *window, vec3 initialCameraPos);
            mat4 getViewMatrix(); 
            mat4 getProjectionMatrix(); 
            void setup(GLFWwindow *window);  
            void update(GLFWwindow *window);    

            static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
            static void mouseButtonCallback( GLFWwindow * window, int button, int action, int mods );
            static void cursorCallback( GLFWwindow *window, double x, double y );
            static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    };

#endif