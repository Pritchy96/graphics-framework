#include "graphics-framework/window_handler.hpp"	
#include "graphics-framework/viewport.hpp"
#include <glm/fwd.hpp>
#include <memory>
#include <vector>

using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::make_unique;

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
		glfwDestroyWindow(glfw_window);
	glfwSetWindowUserPointer(glfw_window, nullptr);
	exit(EXIT_SUCCESS);
}

void WindowHandler::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	// for (InputHandler* i : input_handlers) {
    // 	i->MouseButtonCallback(window, button, action, mods);
	// }


	/* 
	if (focused_viewport_ != nullptr) {
		//this is a continuation of an input, i.e a mouse drag while a mouse button is down.
		//If the mosue button is down (for example), the input should still go to the viewport 
		//initially clicked on and not the viewport the mouse might be over now.
	}
	*/
}
 
void WindowHandler::CursorCallback( GLFWwindow *window, double x, double y ) {
	// for (InputHandler* i : input_handlers) {
    // 	i->CursorCallback(window, x, y);
	// }
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
	// switch (key) {
	// 	case(GLFW_KEY_V) :
	// 		if (action == GLFW_PRESS) {
	// 			GLint mode[2];	//NOLINT: C style array required for glGetIntergerv
	// 			glGetIntegerv(GL_POLYGON_MODE, mode);
	// 			if (mode[0] == GL_LINE) {
	// 				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// 				fprintf( stdout, "Switching View to GL_FILL.\n" );
	// 			} else {
	// 				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// 				fprintf( stdout, "Switching View to GL_LINE.\n" );
	// 			}
	// 		}
	// 		break;

	// 	case(GLFW_KEY_C) :
	// 		if(action == GLFW_PRESS) {
	// 			camera->SetProjection(!camera->ortho_not_perspective);
	// 		}
	// 		break;

	// 	case(GLFW_KEY_G) :
	// 		if(action == GLFW_PRESS) {
	// 			//TODO: render special geo like the grid and axis seperately to avoid this janky by index referencing.
	// 			geo_renderable_pairs.at(0).first->visible = !geo_renderable_pairs.at(0).first->visible;
	// 		}
	// 		break;

	// 	default:
	// 		break;
	// } 
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
