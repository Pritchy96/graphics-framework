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
    int left_mouse_button_down_ = 0;
    GLfloat rotate_speed_;
    GLfloat angle_ = 0;
    glm::vec3 prev_pos_ = glm::vec3(0.0f);
    glm::vec3 curr_pos_  = glm::vec3(0.0f);
    glm::vec3 cam_axis_ = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 rotation_matrix_ = glm::mat4(1.0);
 
public:
    Arcball(Camera* camera, int window_width, int window_height, GLfloat rotate_speed = 1.0f);
    glm::vec3 ToScreenCoord( double x, double y );
    
    void MouseButtonCallback(GLFWwindow * window, int button, int action, int mods) override;
    void CursorCallback(GLFWwindow *window, double x, double y) override;

    glm::mat4 CreateViewRotationMatrix() override;
};

#endif