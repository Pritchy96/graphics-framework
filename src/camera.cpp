#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "../include/camera.hpp"

    glm::mat4 viewMatrix, projectionMatrix, defaultView, rotationMatrix;
    float FOV = 90.0f;

    Camera::Camera(glm::vec3 initial_position, glm::vec3 initial_target, glm::vec3 initial_up) {
        this->position     = initial_position;
        this->target       = initial_target;
        this->up           = initial_up;

        //Projection matrix : Field of View, Aspect ratio, min and max render distance.
        //TODO: make these externally modifiable, regenerate matrix when i.e FoV is changed.
        projectionMatrix = glm::perspective(FOV, 4.0f/3.0f, 0.1f, 10000000.0f);
        viewMatrix = glm::lookAt(position, target, up);
    }   

    glm::mat4 Camera::getViewMatrix() {
        //Todo; some way of conditionally re-calculating this (assuming it's expensive to do so).
        viewMatrix = glm::lookAt(position, target, up);

        return viewMatrix;
    }

    glm::mat4 Camera::getProjectionMatrix() {
        return projectionMatrix;
    }
