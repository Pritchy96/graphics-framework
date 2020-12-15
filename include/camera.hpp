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
            glm::mat4 viewMatrix, projectionMatrix, defaultView, rotationMatrix;
            bool orthoNotPerspective;

            Camera(glm::vec3 initial_position, glm::vec3 initial_direction, glm::vec3 initial_up, int window_width, int window_height);
            ~Camera();

            mat4 getViewMatrix(); 
            mat4 getProjectionMatrix();

            void SetProjection(bool isOrthoNotPerspective);

            void  SetZoom(float new_zoom);
            float GetZoom();

            //Set the target to a unit vector from the camera in the requesed direction 
            void setDirection(vec3 direction) {
                 target = position + glm::normalize(direction);   
            }

            //Get the direction of the target from the camera
            vec3 getDirection() {
                return target - position;
            } 
        private: 
            float zoom = 90.0f;
    };

#endif