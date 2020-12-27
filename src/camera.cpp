#include "graphics-framework/camera.hpp"

Camera::Camera(glm::vec3 initial_position, glm::vec3 initial_target, glm::vec3 initial_up) 
    :   position(initial_position), target(initial_target), up(initial_up) {
    SetProjection(true);
    view_matrix = glm::lookAt(position, target, up);
}

void Camera::SetProjection(bool ortho_not_perspective) {
    ortho_not_perspective = ortho_not_perspective;
    if (ortho_not_perspective) {
        //TODO: make these externally modifiable, regenerate matrix when i.e FoV is changed.
        projection_matrix = glm::ortho(-1000.0f, 1000.0f, -1000.0f, 1000.0f, 0.0f, 100000000000.0f);
    } else {
        projection_matrix = glm::perspective(zoom_, 4.0f/3.0f, 0.1f, 100000000000.0f);
    }
}

void Camera::SetZoom(float new_zoom) {
    zoom_ = new_zoom;
    SetProjection(ortho_not_perspective);
}

float Camera::GetZoom() {
    return zoom_;
}

glm::mat4 Camera::GetViewMatrix() {
    //Todo; some way of conditionally re-calculating this (assuming it's expensive to do so).
    view_matrix = glm::lookAt(position, target, up); 
    return view_matrix;
}

glm::mat4 Camera::GetProjectionMatrix() {
    return projection_matrix;
}