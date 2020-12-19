//Figure out where the users focus is, then send over input to the relevant handler.
//Also handle generic requests i.e key shortcuts which should happen regardless of focus.
#include "input_router.hpp"

using namespace glm;
using namespace std;

Viewport *activeViewport;

Viewport* InputRouter::GetActiveViewport() {
	return activeViewport;
}

void InputRouter::SetActiveViewport(Viewport* active_viewport) {
	activeViewport = active_viewport;
	glfwMakeContextCurrent(activeViewport->glfwWindow);
}

InputRouter::InputRouter(Viewport* active_viewport) {
	SetActiveViewport(active_viewport);
}

void InputRouter::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	SetActiveViewport((Viewport*)glfwGetWindowUserPointer(window));
    activeViewport->mouseButtonCallback(window, button, action, mods);
}
 
void InputRouter::cursorCallback(GLFWwindow *window, double x, double y) {
	SetActiveViewport((Viewport*)glfwGetWindowUserPointer(window));
    activeViewport->cursorCallback(window, x, y);
}	

void InputRouter::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	SetActiveViewport((Viewport*)glfwGetWindowUserPointer(window));
	activeViewport->scrollCallback(window, xoffset, yoffset);
}

void InputRouter::windowSizeCallback(GLFWwindow* window, int width, int height) {
	SetActiveViewport((Viewport*)glfwGetWindowUserPointer(window));
	activeViewport->scrollCallback(window, width, height);
}
	
void InputRouter::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) {
		case(GLFW_KEY_ESCAPE) :
			exit(0);
			break;
		default: 
		    // retrieve MyWindow object from glfw window
        	SetActiveViewport((Viewport*)glfwGetWindowUserPointer(window));
			activeViewport->keyCallback(window, key, scancode, action, mods);
		break;
	}
}