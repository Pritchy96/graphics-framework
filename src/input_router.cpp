//Figure out where the users focus is, then send over input to the relevant handler.
//Also handle generic requests i.e key shortcuts which should happen regardless of focus.
#include "input_router.hpp"

using namespace glm;
using namespace std;

std::weak_ptr<Viewport> activeViewport;

weak_ptr<Viewport> InputRouter::GetActiveViewport() {
	return activeViewport;
}

//Retuns false if window does not have a valid associated viewport.
//TODO: do we need to have a reference to the currently active viewport?
void InputRouter::SetActiveViewport(shared_ptr<Viewport> viewport) {
	activeViewport = viewport;
	glfwMakeContextCurrent(viewport->glfwWindow);
}

InputRouter::InputRouter(shared_ptr<Viewport> active_viewport) {
	SetActiveViewport(active_viewport);
}

void InputRouter::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	Viewport* viewport = (Viewport*)glfwGetWindowUserPointer(window);
	SetActiveViewport(viewport->getSharedPtr());
    viewport->mouseButtonCallback(window, button, action, mods);
}
 
void InputRouter::cursorCallback(GLFWwindow *window, double x, double y) {
	Viewport* viewport = (Viewport*)glfwGetWindowUserPointer(window);
	SetActiveViewport(viewport->getSharedPtr());
    viewport->cursorCallback(window, x, y);
}	

void InputRouter::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	Viewport* viewport = (Viewport*)glfwGetWindowUserPointer(window);
	SetActiveViewport(viewport->getSharedPtr());
	viewport->scrollCallback(window, xoffset, yoffset);
}

void InputRouter::windowSizeCallback(GLFWwindow* window, int width, int height) {
	Viewport* viewport = (Viewport*)glfwGetWindowUserPointer(window);
	SetActiveViewport(viewport->getSharedPtr());
	viewport->windowSizeCallback(window, width, height);
}
	
void InputRouter::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) {
		case(GLFW_KEY_ESCAPE) :
			exit(0);
			break;
		default: 
		    Viewport* viewport = (Viewport*)glfwGetWindowUserPointer(window);
			SetActiveViewport(viewport->getSharedPtr());
			viewport->keyCallback(window, key, scancode, action, mods);
		break;
	}
}