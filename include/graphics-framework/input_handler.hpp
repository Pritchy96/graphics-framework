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
    int window_width;
    int window_height;
public:
    InputHandler(Camera* viewportCamera, int window_width, int window_height) 
        : camera(viewportCamera), window_width(window_width), window_height(window_height)  {
    };
    
    virtual void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset) {};
    virtual void CursorCallback(GLFWwindow *window, double x, double y) {};
    virtual void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {};
    
    void WindowSizeCallback(GLFWwindow* glfwWindow, int width, int height) {
        this->window_height = height;
        this->window_width  = width; 
    }       

    virtual glm::mat4 CreateViewRotationMatrix() = 0;
};

#endif 