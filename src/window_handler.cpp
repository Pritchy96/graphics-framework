#include "graphics-framework/window_handler.hpp"	
#include "GLFW/glfw3.h"
#include "graphics-framework/viewport.hpp"
#include <glm/fwd.hpp>
#include <memory>
#include <unistd.h>
#include <vector>

using std::make_shared;

using glm::vec3;

const glm::vec3 DEFAULT_WINDOW_COLOUR = glm::vec3(0.7f, 0.7f, 0.7f);

void WindowHandler::SetFpsCounter(GLFWwindow* glfw_window, double deltaTime) {
	time_elapsed += deltaTime;
	frames_elapsed++;

	//If it's more than a quarter of a second, update fps.
	if (time_elapsed > 250) {
		double fps = (frames_elapsed / time_elapsed) * 1000;
		char tmp[128];
		//Write formatted data to tmp string.
		snprintf(tmp, sizeof(tmp)/sizeof(char), "Render Window @ %.2f FPS", fps);
		//Set glfw_window title to string.
		glfwSetWindowTitle(glfw_window, tmp);
		frames_elapsed = 0;
		time_elapsed = 0;
	}
}

WindowHandler::WindowHandler(GLFWwindow *window) {
    std::cout << "Initialised Window" << std::endl;
	glfw_window = window;

	glfwGetWindowSize(glfw_window, &width, &height);

	if( !glfw_window ) {
		fprintf(stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
	}

	//TODO: Move everything dependent on this (gl calls, shader loads etc) to an init() function so this context setting can be done 
	//in main.cpp
	glfwMakeContextCurrent(glfw_window);

	//TODO: is this viewport constrained?
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glPointSize(2);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS); //Depth-testing interprets a smaller value as "closer"
	glEnable(GL_SCISSOR_TEST);

	// Initialize GLEW
	glewExperimental = 1u; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
	}

	//TODO: file paths are currently relative to excution path, not main location.
	basic_shader = shader::LoadShaders((char*)"./shaders/basic.vertshader", (char*)"./shaders/basic.fragshader");

	//TEMP test code?
	viewports.push_back(make_shared<Viewport>(glfw_window, DEFAULT_WINDOW_COLOUR, width, height, 0, 0, 0.5f, 1.0f));

	viewports.push_back(make_shared<Viewport>(glfw_window, DEFAULT_WINDOW_COLOUR, width, height, 0.5f, 0, 0.5f, 1.0f));
	viewports.at(1)->camera->position = glm::vec3(0, 0, 10);	//NOLINT: magic numbers ok, temp test code.

}

void WindowHandler::SetupTransformShader(GLuint transformShader) {
	t_shader = transformShader;
}

void WindowHandler::Update(double deltaTime) {
	auto viewport = viewports.begin();

	while(viewport != viewports.end()) {
		//TODO: handle viewport nuke logic?

		(*viewport)->Update(deltaTime);
		viewport++;
	}

	glfwSwapBuffers(glfw_window);
}

WindowHandler::~WindowHandler() {
	// glfwDestroyWindow(glfw_window);
	glfwSetWindowUserPointer(glfw_window, nullptr);
	exit(EXIT_SUCCESS);
}

void WindowHandler::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	auto viewport = viewports.begin();

	while(viewport != viewports.end()) {
		//Draw in the section of the window this viewport encompasses
		int x0 = (width * (*viewport)->x_origin_ratio);
		int y0 = (height * (*viewport)->y_origin_ratio);
		int x1 = x0 + (width * (*viewport)->width_ratio);
		int y1 = y0 + (height * (*viewport)->height_ratio);

		//if viewport is in cursor bounds and we click, pass the callback through
		//and make it the focused viewport.
		//TODO: should some of these be >= or <=?
		if (cursor_x > x0 && cursor_x < x1 && cursor_y > y0 && cursor_y < y1) {
			if (action == GLFW_PRESS) {
				focused_viewport_ = (*viewport);
			}

			(*viewport)->MouseButtonCallback(window, button, action, mods);

			return;
		}
		viewport++;
	}
}
 
void WindowHandler::CursorCallback( GLFWwindow *window, double x, double y ) {
	cursor_x = x;
	cursor_y = y;
	
	if (focused_viewport_ != nullptr) {
		focused_viewport_->CursorCallback(window, x, y);
	}
}

void WindowHandler::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	// //Only allow zoom out if we're really close to the camera target
	// if (glm::length(camera->position - camera->target) > 4.0f || yoffset > 0) {
	// 	//TODO: properly implement a zoom function
	// 	// camera->SetZoom(camera->GetZoom() + (yoffset*1.0f));
	// }
	
	// for (InputHandler* i : input_handlers) {
	// 	i->ScrollCallback(window, xoffset, yoffset);
	// }
}
	
void WindowHandler::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {	//NOLINT: unnused callback params
	if (focused_viewport_ != nullptr) {
		focused_viewport_->KeyCallback(window, key, scancode, action, mods);
	}
}

void WindowHandler::WindowSizeCallback(GLFWwindow* glfw_window, int width, int height) {
	this->height = height;
	this->width = width;

	auto viewport = viewports.begin();

	while(viewport != viewports.end()) {
		(*viewport)->WindowSizeCallback(glfw_window, width, height);
		viewport++;
	}
}
