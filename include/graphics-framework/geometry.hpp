#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

    // #include "stdafx.h"
    #include <vector>
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
    #define GLM_ENABLE_EXPERIMENTAL
    #include <glm/glm.hpp>
    #include <glm/gtc/matrix_transform.hpp>
    #include <glm/gtx/transform.hpp>

    using namespace glm;
    using namespace std;

    class Geometry {
        public:
            Geometry() {};
            Geometry(vector<glm::vec3> vert_data);
            Geometry(vector<glm::vec3> vert_data, vector<glm::vec3> colour_data);

            void Update(float deltaT);

            virtual ~Geometry() {}
            virtual int GenerateFlatBuffers();

            //TODO: replace these with accessors into flatverts.
            vector<vec3> vertexes, colours;
            vector<float> flatVerts, flatCols;
            //If true, the flatvert buffers no longer match the vert buffers
            bool buffersInvalid = true;
            bool visible = true, isDead = false;
    };

#endif