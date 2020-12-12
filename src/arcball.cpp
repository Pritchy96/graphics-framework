#include <glm/gtx/rotate_vector.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include "../include/arcball.hpp"
#include "../include/camera.hpp"

Arcball::Arcball(Camera* camera, int window_width, int window_height, GLfloat rotate_speed) : InputHandler(camera, window_width, window_height) {    
    this->leftMouseButtonDown = 0;
    this->rotateSpeed  = rotate_speed;
    this->angle      = 0.0f;
    this->camAxis    = glm::vec3(0.0f, 1.0f, 0.0f);
}

/**
 * Convert the mouse cursor coordinate on the window (i.e. from (0,0) to (windowWidth, windowHeight))
 * into normalized screen coordinates (i.e. (-1, -1) to (1, 1)
 */
glm::vec3 Arcball::toScreenCoord(double x, double y) {
    glm::vec3 coord(0.0f);
    
    coord.x =  (2 * x - windowWidth ) / windowWidth;
    coord.y = -(2 * y - windowHeight) / windowHeight;
    
    /* Clamp it to border of the windows, comment these codes to allow rotation when cursor is not over window */
    coord.x = glm::clamp(coord.x, -1.0f, 1.0f);
    coord.y = glm::clamp(coord.y, -1.0f, 1.0f);
    
    float length_squared = coord.x * coord.x + coord.y * coord.y;
    if( length_squared <= 1.0 )
        coord.z = sqrt(1.0 - length_squared);
    else
        coord = glm::normalize( coord );
    
    return coord;
}

void Arcball::mouseButtonCallback( GLFWwindow * window, int button, int action, int mods ){
    leftMouseButtonDown = ( action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT );
}

void Arcball::cursorCallback( GLFWwindow *window, double x, double y ){
    currPos = toScreenCoord( x, y );

    if( leftMouseButtonDown == 1 && prevPos != currPos) {
    /* Calculate the angle in radians, and clamp it between 0 and 90 degrees */
    angle = acos(glm::dot(prevPos, currPos));
    
    /* Cross product to get the rotation axis, but it's still in camera coordinate */
    camAxis = glm::cross(prevPos, currPos);

    //Subtract target from camera position - this gives us the vector difference between the camera pos and its target.
    vec3 cameraPosOffset = camera->position - camera->target;
    //Rotate vector
    cameraPosOffset = cameraPosOffset * mat3(createViewRotationMatrix());
    //Set Position to target + rotated vector.
    camera->position = (camera->target + cameraPosOffset);

    } else if (prevPos == currPos) {    
       angle = 0;
    }
    
    prevPos = currPos;
}

/**
 * Create rotation matrix within the camera coordinate,
 * multiply this matrix with view matrix to rotate the camera
 */
glm::mat4 Arcball::createViewRotationMatrix() {
        return glm::rotate( glm::degrees(angle) * rotateSpeed, camAxis );
}

