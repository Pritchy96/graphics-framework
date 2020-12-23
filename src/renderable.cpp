#include "renderable.hpp"

using namespace glm;
using namespace std;

GLuint Renderable::getVAO() {
	if (!validVAO) {

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glGenBuffers(1, &pos_vbo); 
		glGenBuffers(1, &col_vbo);

		glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glBufferData(GL_ARRAY_BUFFER, geometry->flatVerts.size() * sizeof(float), geometry->flatVerts.data(), GL_STREAM_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, col_vbo);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glBufferData(GL_ARRAY_BUFFER, geometry->flatCols.size() * sizeof(float), geometry->flatCols.data(), GL_STATIC_DRAW);

		//Deselect VAO (good practice)
		glBindVertexArray(0);

		validVAO = true;
	}
	return vao;
}

void Renderable::Draw(float deltaT, glm::mat4 projectionMatrix, glm::mat4 viewMatrix){
		if (!geometry->visible)
			return;

		glUseProgram(shader);

		GLuint shaderID = glGetUniformLocation(shader, "scale");
		
		//TODO: Pass through and do multiplication GPU side?
		glm::mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;
		shaderID = glGetUniformLocation(shader, "MVP"); 
		glUniformMatrix4fv(shaderID, 1, GL_FALSE, &MVP[0][0]);
	
		glBindVertexArray(getVAO());
		glDrawArrays(renderType, 0, geometry->vertexes.size());
}

Renderable::Renderable(GLuint Shader, shared_ptr<Geometry> geo_ptr, GLuint renderPrimative) {
	shader = Shader;
	geometry = geo_ptr;
	renderType = renderPrimative;
}