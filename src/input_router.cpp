//Figure out where the users focus is, then send over input to the relevant handler.
//Also handle generic requests i.e key shortcuts which should happen regardless of focus.
#include "input_router.hpp"

using namespace glm;
using namespace std;

Viewport *activeRenderer;

InputRouter::InputRouter(Viewport default_renderer) {
	activeRenderer = &default_renderer;
}

void InputRouter::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    activeRenderer->mouseButtonCallback(window, button, action, mods);
}
 
void InputRouter::cursorCallback(GLFWwindow *window, double x, double y) {
    activeRenderer->cursorCallback(window, x, y);
}	

void InputRouter::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	activeRenderer->scrollCallback(window, xoffset, yoffset);
}
	
void InputRouter::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) {
		case(GLFW_KEY_ESCAPE) :
			exit(0);
			break;
		default:
			activeRenderer->keyCallback(window, key, scancode, action, mods);
	}
}