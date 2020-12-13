#include "camera.hpp"

    glm::mat4 viewMatrix, projectionMatrix, defaultView, rotationMatrix;
    float FOV = 90.0f;

    Camera::Camera(glm::vec3 initial_position, glm::vec3 initial_target, glm::vec3 initial_up) 
        :   position(initial_position), target(initial_target), up(initial_up) {
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
