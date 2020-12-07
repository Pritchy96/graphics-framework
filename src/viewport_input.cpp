// #include "stdafx.h"
#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../include/viewport_input.hpp"

#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL

using namespace glm;
using namespace std;

vec3 cameraPos, cameraPosDefault;
mat4 viewMatrix, projectionMatrix, defaultView, rotationMatrix;
float horizontalMouseDelta = 3.14f, verticalMouseDelta = 1.0f, initialCameraFOV = 45.0f;
float mouseCoef = 0.001f;
float translationSpeed = 300.0f; 
double lastFrameTime, mouseXLastFrame, mouseYLastFrame;
double currentFrameTime;
double yaw = 0.0, pitch = 0.0, zoom = 100.0f;
// Arcball* camera;
float cameraDistance = 30.0f;

mat4 ViewportInput::getViewMatrix() {
	return viewMatrix;
}

mat4 ViewportInput::getProjectionMatrix() {
	return projectionMatrix;
}

ViewportInput::ViewportInput(vec3 initialCameraPos) {
		currentFrameTime = glfwGetTime();
		lastFrameTime = glfwGetTime();
		cameraPosDefault = cameraPos = initialCameraPos;
		//glfwGetWindowSize(window, &windowWidth, &windowHeight);
		//camera = new Arcball( windowWidth, windowHeight, .015f, true, true );

		float FoV = initialCameraFOV;

		//Projection matrix : 45 Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		projectionMatrix = glm::perspective(70.0f, 4.0f/3.0f, 0.1f, 10000000.0f);
		rotationMatrix = glm::mat4(1.0);
}

void ViewportInput::update(GLFWwindow *window){
	currentFrameTime = glfwGetTime();
	float deltaTime = float(currentFrameTime - lastFrameTime);

	// if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		// camera->update(deltaTime);
		
		// Camera matrix
		// rotationMatrix *= camera->createViewRotationMatrix();
		viewMatrix = lookAt(glm::vec3(0.50f, 0.0f, cameraDistance), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0)) * rotationMatrix;
	
	// }

	// For the next frame, the "last time" will be "now"
	lastFrameTime = currentFrameTime;
}
void ViewportInput::mouseButtonCallback( GLFWwindow* window, int button, int action, int mods ){
    // camera->mouseButtonCallback(window, button, action, mods);
}
 
void ViewportInput::cursorCallback( GLFWwindow *window, double x, double y ) {
    // camera->cursorCallback(window, x, y);
}

void ViewportInput::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	cameraDistance = std::fmax(1.0, cameraDistance+yoffset);
}
	
void ViewportInput::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
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