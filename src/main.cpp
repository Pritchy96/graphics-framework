#include <iostream>
#include <chrono>

#include <boost/filesystem.hpp>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL //gtx = gt eXperimental?
#include <glm/gtx/string_cast.hpp>

#include "../include/viewport.hpp"
#include "../include/input_handler.hpp"
#include "../include/shader.hpp"

using namespace std;
using namespace boost;

vector<vec3> test_data_lines = {
	vec3(0.000000, 4.000000, 12.000000),
	vec3(2.870316, -101.411970, 1.000000),
};

auto oldTime = chrono::steady_clock::now(), newTime = chrono::steady_clock::now();
double deltaT;	
Viewport *renderer;	
InputHandler *inputHandler;

int main(int argc, const char* argv[]) {

	srand(time(NULL));
    cout << "Launching Program" << endl;

    renderer = new Viewport(glm::vec3(0.7f, 0.7f, 0.7f));
    cout << "Initialised Viewport" << endl;

	glfwSetInputMode(renderer->glfwWindow, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetKeyCallback(renderer->glfwWindow, inputHandler->keyCallback);
	glfwSetMouseButtonCallback(renderer->glfwWindow, inputHandler->mouseButtonCallback);
	glfwSetScrollCallback(renderer->glfwWindow, inputHandler->scrollCallback);
	glfwSetCursorPosCallback(renderer->glfwWindow, inputHandler->cursorCallback);

	GLuint shader = Shader::LoadShaders("./bin/shaders/basic.vertshader", "./bin/shaders/basic.fragshader");
    renderer->addRenderable(new Renderable(shader, test_data_lines, test_data_lines, GL_LINES));
	
    while (true) {  //TODO: Write proper update & exit logic.
		oldTime = newTime;
    	newTime = chrono::steady_clock::now();
		deltaT = chrono::duration_cast<chrono::milliseconds>(newTime - oldTime).count();

        renderer->update(deltaT);

        //Update other events like input handling 
	    glfwPollEvents();
    }

    return 0;
}