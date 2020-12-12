#ifndef RENDERENVIRONMENT_HPP
#define RENDERENVIRONMENT_HPP

    // #include "stdafx.h"
    #include <vector>


    #include <GLFW/glfw3.h>
    #include <glm/glm.hpp>
    #include <glm/gtc/matrix_transform.hpp>
    #include "../include/renderable.hpp"

    using namespace glm;
    using namespace std;

    class Viewport {
        public:
            Viewport(glm::vec3 backgroundColour);
            ~Viewport();

            void addRenderable(Renderable* renderable);
            void update(float deltaT);
            void setupTransformShader(GLuint transformShader);   
            
            static void errorCallback(int error, const char* description);
            static void windowSizeCallback(GLFWwindow* window, int width, int height);
            static void setFPSCounter(GLFWwindow* window, double deltaT);

            void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
            void mouseButtonCallback( GLFWwindow* window, int button, int action, int mods );
            void cursorCallback( GLFWwindow* window, double x, double y );
            void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

            GLFWwindow* glfwWindow;
            vector<Renderable*> renderables;
            GLuint tShader;      
    };

#endif