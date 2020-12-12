#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "../include/viewport.hpp"	

#include "../include/shader.hpp"
#include "../include/renderable.hpp"
#include "../include/arcball.h"
#include "../include/camera.hpp"

using namespace std;
using namespace glm;


GLuint shaderID;
GLuint basicShader;

int gl_width = 1024;
int gl_height = 768;

double timeElapsed = 0;
int framesElapsed = 0;

Arcball* arcballCamera;
Camera* camera;

Renderable* renderAxis;
vector<vec3> axis_lines = {
    vec3(0.0f, 0.0f, 0.0f),	//x
	vec3(100.0f, 0.0f, 0.0f),
	vec3(0.0f, 0.0f, 0.0f),	//y	
	vec3(0.0f, 100.0f, 0.0f), 
	vec3(0.0f, 0.0f, 0.0f),	//z
	vec3(0.0f, 0.0f, 100.0f)
};

vector<vec3> axis_colours = {
    vec3(1.0f, 0.0f, 0.0f),	//x
	vec3(1.0f, 0.0f, 0.0f),	
	vec3(0.0f, 1.0f, 0.0f), //y
	vec3(0.0f, 1.0f, 0.0f),
	vec3(0.0f, 0.0f, 1.0f),	//z
	vec3(0.0f, 0.0f, 1.0f)
};

void Viewport::setFPSCounter(GLFWwindow* glfwWindow, double deltaTime) {
	timeElapsed += deltaTime;
	framesElapsed++;

	//If it's more than a quarter of a second, update fps.
	if (timeElapsed > 250) {
		double fps = (double)(framesElapsed / timeElapsed) * 1000;
		char tmp[128];
		//Write formatted data to tmp string.
		snprintf(tmp, sizeof(tmp)/sizeof(char), "Render Window @ %.2f FPS", fps);
		//Set glfwWindow title to string.
		glfwSetWindowTitle(glfwWindow, tmp);
		framesElapsed = 0;
		timeElapsed = 0;
	}
}

Viewport::Viewport(glm::vec3 backgroundColour) {
	if( !glfwInit() ) {
		fprintf( stderr, "Failed to initialize GLFW\n" );
	}

	glfwSetErrorCallback(errorCallback);

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //For MacOS compat, apparrently 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	glfwWindow = glfwCreateWindow(gl_width, gl_height, "Render Window", NULL, NULL);

	camera = new Camera(glm::vec3(100, 100, 100), glm::vec3(0, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f));
	arcballCamera = new Arcball(camera, gl_width, gl_height, .015f);

	if( !glfwWindow ) {
		fprintf(stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
	}

	glfwSetWindowSizeCallback(glfwWindow, windowSizeCallback);
	glfwMakeContextCurrent(glfwWindow);

	glClearColor(backgroundColour.r, backgroundColour.g, backgroundColour.b, 0.01f);
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glPointSize(2);
	glLineWidth(4); //This doesn't work?

	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS); //Depth-testing interprets a smaller value as "closer"

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
	}
	
	basicShader = Shader::LoadShaders("./bin/shaders/basic.vertshader", "./bin/shaders/basic.fragshader");
    renderAxis = new Renderable(basicShader, axis_lines, axis_colours, GL_LINES);
	addRenderable(renderAxis);
}

void Viewport::addRenderable(Renderable* renderable) {
	renderables.push_back(renderable);
}

void Viewport::setupTransformShader(GLuint transformShader) {
	tShader = transformShader;
}

void Viewport::update(float deltaTime) {
	setFPSCounter(glfwWindow, deltaTime);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	vector<Renderable*>::iterator renderable = renderables.begin();

	while(renderable != renderables.end()) {
		if((*renderable)->isDead) {
			delete(*renderable);
			//iterator.erase gives the next item in the list.
			renderable = renderables.erase(renderable);
		} else {
			(*renderable)->Draw(deltaTime, camera->getProjectionMatrix(), camera->getViewMatrix());
			++renderable;
		}
	}
	glfwSwapBuffers(glfwWindow);
}

Viewport::~Viewport() {
	glfwDestroyWindow(glfwWindow);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void Viewport::mouseButtonCallback( GLFWwindow* window, int button, int action, int mods ){
    arcballCamera->mouseButtonCallback(window, button, action, mods);
}
 
void Viewport::cursorCallback( GLFWwindow *window, double x, double y ) {
    arcballCamera->cursorCallback(window, x, y);
}

void Viewport::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	//Only allow zoom out if we're really close to the camera target
	if (glm::length(camera->position - camera->target) > 4.0f || yoffset > 0)
		camera->position = camera->position + (glm::normalize(camera->position - camera->target) * (float)yoffset * 1.0f);
}
	
void Viewport::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) {
		case(GLFW_KEY_V) :
			if (action == GLFW_PRESS) {
				GLint mode[2];
				glGetIntegerv(GL_POLYGON_MODE, mode);
				if (mode[0] == GL_LINE) {
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					fprintf( stdout, "Switching View to GL_FILL.\n" );
				} else {
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					fprintf( stdout, "Switching View to GL_LINE.\n" );
				}
			}
			break;
		default:
			break;
	} 
}

void Viewport::errorCallback(int error, const char* description) {
	fprintf(stderr, description);
}

void Viewport::windowSizeCallback(GLFWwindow* glfwWindow, int width, int height) {
	gl_height = height;
	gl_width = width;
	glViewport(0, 0, gl_width, gl_height);
}