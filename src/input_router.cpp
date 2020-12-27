//Figure out where the users focus is, then send over input to the relevant handler.
//Also handle generic requests i.e key shortcuts which should happen regardless of focus.
#include <memory>
#include <utility>

#include "graphics-framework/input_router.hpp"

using std::shared_ptr;
using std::weak_ptr;

std::weak_ptr<Viewport> active_viewport;

weak_ptr<Viewport> InputRouter::GetActiveViewport() {
	return active_viewport;
}

//Retuns false if window does not have a valid associated viewport.
//TODO: do we need to have a reference to the currently active viewport?
void InputRouter::SetActiveViewport(shared_ptr<Viewport> viewport) {
	active_viewport = viewport;
	glfwMakeContextCurrent(viewport->glfw_window);
}

InputRouter::InputRouter(shared_ptr<Viewport> active_viewport) {
	SetActiveViewport(std::move(active_viewport));
}

void InputRouter::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	auto* viewport = static_cast<Viewport*>(glfwGetWindowUserPointer(window));
	SetActiveViewport(viewport->GetSharedPtr());
    viewport->MouseButtonCallback(window, button, action, mods);
}

void InputRouter::CursorCallback(GLFWwindow *window, double x, double y) {
	auto* viewport = static_cast<Viewport*>(glfwGetWindowUserPointer(window));
	SetActiveViewport(viewport->GetSharedPtr());
    viewport->CursorCallback(window, x, y);
}

void InputRouter::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	auto* viewport = static_cast<Viewport*>(glfwGetWindowUserPointer(window));
	SetActiveViewport(viewport->GetSharedPtr());
	viewport->ScrollCallback(window, xoffset, yoffset);
}

void InputRouter::WindowSizeCallback(GLFWwindow* window, int width, int height) {
	auto* viewport = static_cast<Viewport*>(glfwGetWindowUserPointer(window));
	SetActiveViewport(viewport->GetSharedPtr());
	viewport->WindowSizeCallback(window, width, height);
}

void InputRouter::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) {
		case(GLFW_KEY_ESCAPE) :
			//TODO: move to viewport, escape kills currently focused viewport, another key adds one.
			exit(0);
			break;
		default: 
		    auto* viewport = static_cast<Viewport*>(glfwGetWindowUserPointer(window));
			SetActiveViewport(viewport->GetSharedPtr());
			viewport->KeyCallback(window, key, scancode, action, mods);
		break;
	}
}