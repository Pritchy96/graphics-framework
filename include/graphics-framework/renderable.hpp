#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

    // #include "stdafx.h"
    #include <vector>
    #include <memory>

    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
    #define GLM_ENABLE_EXPERIMENTAL
    #include <glm/glm.hpp>
    #include <glm/gtc/matrix_transform.hpp>
    #include <glm/gtx/transform.hpp>
    
    #include "geometry.hpp"

    using namespace glm;
    using namespace std;

    class Renderable {
        public:
            Renderable(GLuint Shader, shared_ptr<Geometry> geo_ptr, GLuint renderPrimative = GL_POINTS);
            Renderable() = default;;
            virtual GLuint GetVAO();
            virtual void Draw(float deltaT, glm::mat4 projectionMatrix, glm::mat4 viewMatrix);

            shared_ptr<Geometry> geometry;
	        GLuint pos_vbo, col_vbo, vao, shader, render_type;
            glm::mat4 model_matrix = glm::mat4(1.0f);

            bool valid_vao = false, is_dead = false;
        private:
            int vert_buffer_size_;
    };

#endif