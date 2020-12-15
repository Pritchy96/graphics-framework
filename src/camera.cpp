#include "camera.hpp"



Camera::Camera(glm::vec3 initial_position, glm::vec3 initial_target, glm::vec3 initial_up, int window_width, int window_height) 
    :   position(initial_position), target(initial_target), up(initial_up) {
    //Projection matrix : Field of View, Aspect ratio, min and max render distance.
    //TODO: make these externally modifiable, regenerate matrix when i.e FoV is changed.
    //projectionMatrix = glm::perspective(FOV, 4.0f/3.0f, 0.1f, 10000000.0f);
    SetProjection(true);
    viewMatrix = glm::lookAt(position, target, up);
}

void Camera::SetProjection(bool ortho_not_perspective) {
    orthoNotPerspective = ortho_not_perspective;
    if (orthoNotPerspective)
        projectionMatrix = glm::ortho(-1000.0f, 1000.0f, -1000.0f, 1000.0f, 0.0f, 100000000000.0f);
    else
        projectionMatrix = glm::perspective(zoom, 4.0f/3.0f, 0.1f, 100000000000.0f);
}

void Camera::SetZoom(float new_zoom) {
    zoom = new_zoom;
    SetProjection(orthoNotPerspective);
}

float Camera::GetZoom() {
    return zoom;
}

glm::mat4 Camera::getViewMatrix() {
    //Todo; some way of conditionally re-calculating this (assuming it's expensive to do so).
    viewMatrix = glm::lookAt(position, target, up); 
    return viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix() {
    return projectionMatrix;
}