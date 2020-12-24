#ifndef ARCBALL_HPP
#define ARCBALL_HPP

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "input_handler.hpp"
#include "camera.hpp"

class Arcball : public InputHandler {
private:
    int leftMouseButtonDown = 0;
    GLfloat rotateSpeed;
    GLfloat angle = 0;
    glm::vec3 prevPos = glm::vec3(0.0f);
    glm::vec3 currPos = glm::vec3(0.0f);
    glm::vec3 camAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 rotationMatrix = glm::mat4(1.0);
    
    bool xAxis;
    bool yAxis;
    
public:
    Arcball(Camera* camera, int window_width, int window_height, GLfloat rotate_speed = 1.0f);
    glm::vec3 toScreenCoord( double x, double y );
    
    void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods) override;
    void cursorCallback(GLFWwindow *window, double x, double y) override;

    glm::mat4 createViewRotationMatrix() override;
};

#endif 