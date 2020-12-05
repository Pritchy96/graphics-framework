#ifndef RENDERENVIRONMENT_HPP
#define RENDERENVIRONMENT_HPP

    // #include "stdafx.h"
    #include <vector>

    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
    #include <glm/glm.hpp>
    #include <glm/gtc/matrix_transform.hpp>
    #include "../include/renderable.hpp"

    using namespace glm;
    using namespace std;

    class RenderEnvironment {
        public:
            RenderEnvironment(glm::vec3 backgroundColour);
            ~RenderEnvironment();

            void addRenderable(Renderable* renderable);
            void update(float deltaT);
            void setupTransformShader(GLuint transformShader);   
            
            static void errorCallback(int error, const char* description);
            static void windowSizeCallback(GLFWwindow* window, int width, int height);
            static void setFPSCounter(GLFWwindow* window, double deltaT);
            
            vector<Renderable*> renderables;
            GLuint tShader;      
    };

#endif