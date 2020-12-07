#ifndef CAMERA_HPP
#define CAMERA_HPP

    #include <iostream>

    #include <GLFW/glfw3.h>

    #include <glm/glm.hpp>
    #include <glm/gtc/type_ptr.hpp>
    #include <glm/gtc/matrix_transform.hpp>
    #include <glm/gtx/rotate_vector.hpp>
    #include <glm/gtc/matrix_inverse.hpp>

    using namespace glm;
    using namespace std;

    class Camera {
        public:
            int windowWidth, windowHeight;
            glm::vec3 position, direction, up, right;   //Everything we need to represent a camera?

            Camera(int window_width, int window_height, glm::vec3 initial_position, glm::vec3 initial_direction, glm::vec3 initial_up, glm::vec3 initial_right);
            ~Camera();

            // glm::vec3 toScreenCoord(double x, double y);
            // void update(float deltaTime) {};
            // glm::mat4 createViewRotationMatrix();
            // glm::mat4 createModelRotationMatrix(glm::mat4& view_matrix);     
    };

#endif