//Figure out where the users focus is, then send over input to the relevant handler.
//Also handle generic requests i.e key shortcuts which should happen regardless of focus.
#include <memory>
#include <utility>

#include "graphics-framework/input_router.hpp"
#include "graphics-framework/window_handler.hpp"

using std::shared_ptr;
using std::weak_ptr;

std::weak_ptr<WindowHandler> active_window_handler;

weak_ptr<WindowHandler> InputRouter::GetActiveWindowHandler() {
	return active_window_handler;
}

//Retuns false if window does not have a valid associated window_handler.
//TODO: do we need to have a reference to the currently active window_handler?
void InputRouter::SetActiveWindowHandler(shared_ptr<WindowHandler> window_handler) {
	active_window_handler = window_handler;
	glfwMakeContextCurrent(window_handler->glfw_window);
}

InputRouter::InputRouter(shared_ptr<WindowHandler> active_window) {
	SetActiveWindowHandler(std::move(active_window));
}

void InputRouter::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	auto* window_handler = static_cast<WindowHandler*>(glfwGetWindowUserPointer(window));
	SetActiveWindowHandler(window_handler->GetSharedPtr());
    window_handler->MouseButtonCallback(window_handler->glfw_window, button, action, mods);
}	

void InputRouter::CursorCallback(GLFWwindow *window, double x, double y) {
	auto* window_handler = static_cast<WindowHandler*>(glfwGetWindowUserPointer(window));
	SetActiveWindowHandler(window_handler->GetSharedPtr());
    window_handler->CursorCallback(window_handler->glfw_window, x, y);
}

void InputRouter::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	auto* window_handler = static_cast<WindowHandler*>(glfwGetWindowUserPointer(window));
	SetActiveWindowHandler(window_handler->GetSharedPtr());
	window_handler->ScrollCallback(window_handler->glfw_window, xoffset, yoffset);
}

void InputRouter::WindowSizeCallback(GLFWwindow* window, int width, int height) {
	auto* window_handler = static_cast<WindowHandler*>(glfwGetWindowUserPointer(window));
	SetActiveWindowHandler(window_handler->GetSharedPtr());
	window_handler->WindowSizeCallback(window_handler->glfw_window, width, height);
}

void InputRouter::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) {
		case(GLFW_KEY_ESCAPE) :
			//TODO: move to window_handler, escape kills currently focused window_handler, another key adds one.
			exit(0);
			break;
		default: 
		    auto* window_handler = static_cast<WindowHandler*>(glfwGetWindowUserPointer(window));
			SetActiveWindowHandler(window_handler->GetSharedPtr());
			window_handler->KeyCallback(window, key, scancode, action, mods);
		break;
	}
}
