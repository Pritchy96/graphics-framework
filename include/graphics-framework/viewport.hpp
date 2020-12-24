#ifndef VIEWPORT_HPP
#define VIEWPORT_HPP

    #include <iostream>
    #include <stdio.h>
    #include <stdlib.h>
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

    using namespace glm;
    using namespace std;

    class Viewport: public enable_shared_from_this<Viewport> {
        public:
            Viewport(GLFWwindow *window, glm::vec3 background_colour);
            ~Viewport();

            shared_ptr<Viewport> getSharedPtr() {
                return shared_from_this();
            }

            void update(float deltaT);  
            void setupTransformShader(GLuint transformShader);   
            
            void windowSizeCallback(GLFWwindow* window, int width, int height);
            void setFPSCounter(GLFWwindow* window, double deltaT);
            void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
            void mouseButtonCallback( GLFWwindow* window, int button, int action, int mods );
            void cursorCallback( GLFWwindow* window, double x, double y );
            void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

            GLFWwindow *glfwWindow;
            vector<pair<shared_ptr<Geometry>, shared_ptr<Renderable>>> geoRenderablePairs;

            GLuint tShader;      
            GLuint shaderID;    
            GLuint basicShader;

            int width = -1, height = -1;

            double timeElapsed = 0;
            int framesElapsed = 0;

            vector<InputHandler*> inputHandlers;
            Arcball* arcballCamera;
            Camera* camera;
    };

#endif