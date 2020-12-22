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
            Renderable(GLuint Shader, weak_ptr<Geometry> geo, GLuint renderPrimative = GL_POINTS);
            Renderable() {};
            virtual GLuint getVAO();
            virtual void Draw(float deltaT, glm::mat4 projectionMatrix, glm::mat4 viewMatrix);

            weak_ptr<Geometry> geoPtr;
	        GLuint pos_vbo, col_vbo, vao, shader, renderType;
            glm::mat4 modelMatrix = glm::mat4(1.0f);

            bool validVAO = false, isDead = false;
        private:
            int vertBufferSize;
    };

#endif