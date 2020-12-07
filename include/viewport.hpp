#ifndef RENDERENVIRONMENT_HPP
#define RENDERENVIRONMENT_HPP

    // #include "stdafx.h"
    #include <vector>


    #include <GLFW/glfw3.h>
    #include <glm/glm.hpp>
    #include <glm/gtc/matrix_transform.hpp>
    #include "../include/renderable.hpp"

    #include "../include/viewport_input.hpp"

    using namespace glm;
    using namespace std;

    class Viewport : public ViewportInput {
        public:
            Viewport(glm::vec3 backgroundColour);
            ~Viewport();

            void addRenderable(Renderable* renderable);
            void update(float deltaT);
            void setupTransformShader(GLuint transformShader);   
            
            static void errorCallback(int error, const char* description);
            static void windowSizeCallback(GLFWwindow* window, int width, int height);
            static void setFPSCounter(GLFWwindow* window, double deltaT);

            GLFWwindow* glfwWindow;
            vector<Renderable*> renderables;
            GLuint tShader;      
    };

#endif