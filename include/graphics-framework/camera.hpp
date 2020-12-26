#ifndef CAMERA_HPP
#define CAMERA_HPP

    #include <iostream>
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
    #include <glm/glm.hpp>
    #include <glm/gtc/type_ptr.hpp>
    #include <glm/gtc/matrix_transform.hpp>
    #include <glm/gtx/rotate_vector.hpp>
    #include <glm/gtc/matrix_inverse.hpp>

    using namespace glm;    
    using namespace std;

    struct Camera {
        public:
            glm::vec3 position, target, up;  
            glm::mat4 view_matrix, projection_matrix, default_view, rotation_matrix;
            bool ortho_not_perspective;

            Camera(glm::vec3 initial_position, glm::vec3 initial_target, glm::vec3 initial_up);
            ~Camera();

            mat4 GetViewMatrix(); 
            mat4 GetProjectionMatrix();

            void SetProjection(bool ortho_not_perspective);

            void  SetZoom(float new_zoom);
            float GetZoom();

            //Set the target to a unit vector from the camera in the requesed direction 
            void SetDirection(vec3 direction) {
                 target = position + glm::normalize(direction);   
            }

            //Get the direction of the target from the camera
            vec3 GetDirection() const {
                return target - position;
            }   
        private: 
            float zoom_ = 90.0f;
    };

#endif