#ifndef RENDERENVIRONMENT_HPP
#define RENDERENVIRONMENT_HPP

    #include <iostream>
    #include <stdio.h>
    #include <stdlib.h>
    #include <vector>

    #define GLM_ENABLE_EXPERIMENTAL
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
    #include <glm/glm.hpp>
    #include <glm/gtc/matrix_transform.hpp>
    #include <glm/gtc/type_ptr.hpp>
    #include <glm/gtx/transform.hpp>
    #include <glm/gtx/string_cast.hpp>

    #include "viewport.hpp"	

    #include "shader.hpp"
    #include "renderable.hpp"
    #include "viewport_grid.hpp"
    #include "arcball.hpp"
    #include "input_handler.hpp"
    #include "camera.hpp"

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