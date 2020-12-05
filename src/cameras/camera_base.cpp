#define GLM_ENABLE_EXPERIMENTAL
#include "../../include/CameraBase.h"

/**
 * Constructor.
 * @param roll_speed the speed of rotation
 */
CameraBase::CameraBase( int window_width, int window_height) {
    this->windowWidth  = window_width;
    this->windowHeight = window_height;
}

void CameraBase::update(float deltaTime) {
}

/**
 * Create rotation matrix within the camera coordinate,
 * multiply this matrix with view matrix to rotate the camera
 */
glm::mat4 CameraBase::createViewRotationMatrix() {
    if (leftMouseButtonDown == 0) {
       return glm::mat4(1.0); 
    } else {
        return glm::rotate( glm::degrees(angle) * rollSpeed, camAxis );
    }
}

/**
 * Create rotation matrix within the world coordinate,
 * multiply this matrix with model matrix to rotate the object
 */
glm::mat4 CameraBase::createModelRotationMatrix( glm::mat4& view_matrix ){
    glm::vec3 axis = glm::inverse(glm::mat3(view_matrix)) * camAxis;
    return glm::rotate( glm::degrees(angle) * rollSpeed, axis );
}