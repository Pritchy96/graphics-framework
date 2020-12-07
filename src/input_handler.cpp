//Figure out where the users focus is, then send over input to the relevant handler.
//Also handle generic requests i.e key shortcuts which should happen regardless of focus.

// #include "stdafx.h"
#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../include/input_handler.hpp"

#define GLM_ENABLE_EXPERIMENTAL

using namespace glm;
using namespace std;

Viewport *activeRenderer;
int windowWidth, windowHeight;

InputHandler::InputHandler(Viewport default_renderer) {
    //currentFrameTime = lastFrameTime = glfwGetTime();
    //glfwGetWindowSize(window, &windowWidth, &windowHeight);
    activeRenderer = &default_renderer;
}

void InputHandler::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    activeRenderer->mouseButtonCallback(window, button, action, mods);
}
 
void InputHandler::cursorCallback(GLFWwindow *window, double x, double y) {
    activeRenderer->cursorCallback(window, x, y);
}

void InputHandler::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	activeRenderer->scrollCallback(window, xoffset, yoffset);
}
	
void InputHandler::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) {
		case(GLFW_KEY_ESCAPE) :
			exit(0);
			break;
		default:
			activeRenderer->keyCallback(window, key, scancode, action, mods);
	}
}