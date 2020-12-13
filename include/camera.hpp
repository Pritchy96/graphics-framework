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

            Camera(glm::vec3 initial_position, glm::vec3 initial_direction, glm::vec3 initial_up);
            ~Camera();

            mat4 getViewMatrix(); 
            mat4 getProjectionMatrix();

            //Set the target to a unit vector from the camera in the requesed direction 
            void setDirection(vec3 direction) {
                 target = position + glm::normalize(direction);   
            }

            //Get the direction of the target from the camera
            vec3 getDirection() {
                return target - position;
            } 
    };

#endif