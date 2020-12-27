#include "graphics-framework/arcball.hpp"
#include <glm/fwd.hpp>

using glm::vec3;
using glm::mat3;
using glm::mat4;

Arcball::Arcball(Camera *camera, int window_width, int window_height, GLfloat rotate_speed)
            : InputHandler(camera, window_width, window_height),
                rotate_speed_(rotate_speed) {}

/**
 * Convert the mouse cursor coordinate on the window (i.e. from (0,0) to (windowWidth, windowHeight))
 * into normalized screen coordinates (i.e. (-1, -1) to (1, 1)
 */
vec3 Arcball::ToScreenCoord(double x, double y) {
    vec3 coord(0.0f);
    
    coord.x =  (2 * x - window_width) / window_width;
    coord.y = -(2 * y - window_height) / window_height;
    
    /* Clamp it to border of the windows, comment these codes to allow rotation when cursor is not over window */
    coord.x = glm::clamp(coord.x, -1.0f, 1.0f);
    coord.y = glm::clamp(coord.y, -1.0f, 1.0f);
    
    float length_squared = coord.x * coord.x + coord.y * coord.y;
    if( length_squared <= 1.0 ) {
        coord.z = sqrt(1.0 - length_squared);
    } else {
        coord = normalize( coord );
    }
    
    return coord;
}

void Arcball::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods){    //NOLINT: unnused callback params
    left_mouse_button_down_ = static_cast<int> (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT);
}

void Arcball::CursorCallback( GLFWwindow *window, double x, double y ){ //NOLINT: unnused callback params
    curr_pos_ = ToScreenCoord( x, y );

    if( left_mouse_button_down_ == 1 && prev_pos_ != curr_pos_) {
    /* Calculate the angle in radians, and clamp it between 0 and 90 degrees */
    angle_ = acos(dot(prev_pos_, curr_pos_));
    
    /* Cross product to get the rotation axis, but it's still in camera coordinate */
    cam_axis_ = cross(prev_pos_, curr_pos_);

    //Subtract target from camera position - this gives us the vector difference between the camera pos and its target.
    vec3 camera_pos_offset = camera->position - camera->target;
    //Rotate vector
    camera_pos_offset = camera_pos_offset * mat3(CreateViewRotationMatrix());
    //Set Position to target + rotated vector.
    camera->position = (camera->target + camera_pos_offset);

    } else if (prev_pos_ == curr_pos_) {    
       angle_ = 0;
    }

    prev_pos_ = curr_pos_;
}

/**
 * Create rotation matrix within the camera coordinate,
 * multiply this matrix with view matrix to rotate the camera
 */
mat4 Arcball::CreateViewRotationMatrix() {
  return rotate(glm::degrees(angle_) * rotate_speed_, cam_axis_);
}
