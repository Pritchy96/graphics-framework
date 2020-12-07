#define GLM_ENABLE_EXPERIMENTAL
#include "../include/camera.hpp"

/**
 * Constructor.
 * @param roll_speed the speed of rotation
 */
Camera::Camera(int window_width, int window_height, glm::vec3 initial_position, glm::vec3 initial_direction, glm::vec3 initial_up, glm::vec3 initial_right) {
    this->windowWidth  = window_width;
    this->windowHeight = window_height;
    this->up           = initial_up;
    this->right        = initial_right;
}