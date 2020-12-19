#include <iostream>
#include <chrono>

#include <boost/filesystem.hpp>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "viewport.hpp"
#include "input_router.hpp"
#include "shader.hpp"

using namespace std;
using namespace boost;

vector<vec3> test_data_lines = {
	glm::vec3(00.0, 00.0, 00.0),
	glm::vec3(200.0, 00.0, 00.0),
	glm::vec3(00.0, 200.0, 00.0),

	glm::vec3(00.0, 00.0, 00.0),
	glm::vec3(00.0, 00.0, 200.0),
	glm::vec3(200.0, 00.0, 00.0),

	glm::vec3(00.0, 00.0, 00.0),
	glm::vec3(00.0, 200.0, 00.0),
	glm::vec3(00.0, 00.0, 200.0)

};

auto oldTime = chrono::steady_clock::now(), newTime = chrono::steady_clock::now();
double deltaT;	

vector<Viewport*> renderers;
InputRouter *inputRouter;

Viewport* CreateWindow(int width, int height, char* title, glm::vec3 backgroundCol, GLFWwindow* sharedWindow = NULL) {
	Viewport* viewport = new Viewport(glfwCreateWindow(width, height, title, NULL, sharedWindow), backgroundCol);

	//Bind the viewport class pointer to the window within, so the callbacks can pass through to the non static
	//viewport callback handlers from the static inputRouter callback functions using the provided window.
	glfwSetWindowUserPointer(viewport->glfwWindow, viewport);	

	glfwSetInputMode(viewport->glfwWindow, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetKeyCallback(viewport->glfwWindow, inputRouter->keyCallback);
	glfwSetMouseButtonCallback(viewport->glfwWindow, inputRouter->mouseButtonCallback);
	glfwSetScrollCallback(viewport->glfwWindow, inputRouter->scrollCallback);
	glfwSetCursorPosCallback(viewport->glfwWindow, inputRouter->cursorCallback);
	glfwSetWindowSizeCallback(viewport->glfwWindow, inputRouter->windowSizeCallback);
	
	return viewport;
}

void errorCallback(int error, const char* description) {
	fprintf(stderr, description);
}

//TODO split into init() and mainLoop() functions.
int main(int argc, const char* argv[]) {
    cout << "Launching Program" << endl;
	srand(time(NULL));

	if( !glfwInit() ) {
		fprintf( stderr, "Failed to initialize GLFW\n" );
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //For MacOS compat, apparently
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	inputRouter = new InputRouter();

    renderers.push_back(CreateWindow(1024, 768, "Render Window", glm::vec3(0.7f, 0.7f, 0.7f)));
	inputRouter->SetActiveViewport(renderers.at(0));

	renderers.push_back(CreateWindow(1024, 768, "Render Window", glm::vec3(0.4f, 0.4f, 0.4f)));

	GLuint shader = Shader::LoadShaders((char*)"./bin/shaders/basic.vertshader", (char*)"./bin/shaders/basic.fragshader");

	//TODO: Renderables need to be pulled out to a master list and passed by reference to viewports.
    renderers.at(0)->addRenderable(new Renderable(shader, test_data_lines, test_data_lines, GL_TRIANGLES));
	renderers.at(1)->addRenderable(new Renderable(shader, test_data_lines, test_data_lines, GL_TRIANGLES));

	renderers.at(1)->camera->position = glm::vec3(0, 0, 10);

	glfwSetErrorCallback(errorCallback);

    while (true) {  //TODO: Write proper update & exit logic.
		oldTime = newTime;
    	newTime = chrono::steady_clock::now();
		deltaT = chrono::duration_cast<chrono::milliseconds>(newTime - oldTime).count();

		for (Viewport* v : renderers) {
			inputRouter->SetActiveViewport(v);
        	v->update(deltaT);
			//Update other events like input handling 
	    	glfwPollEvents();
		}
    }

    return 0;
}

