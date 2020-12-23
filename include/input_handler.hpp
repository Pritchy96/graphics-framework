#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "camera.hpp"

class InputHandler {
protected: 
    Camera* camera;
    int windowWidth;
    int windowHeight;
public:
    InputHandler(Camera* viewportCamera, int window_width, int window_height) 
        : camera(viewportCamera), windowWidth(window_width), windowHeight(window_height)  {
    };
    
    virtual void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {};
    virtual void cursorCallback(GLFWwindow *window, double x, double y) {};
    virtual void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {};
    
    void windowSizeCallback(GLFWwindow* glfwWindow, int width, int height) {
        this->windowHeight = height;
        this->windowWidth  = width; 
    }

    virtual glm::mat4 createViewRotationMatrix() = 0;
};

#endif 