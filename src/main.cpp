#include "graphics-framework/window_handler.hpp"
#include <glm/detail/qualifier.hpp>
#include <iostream>
#include <chrono>
#include <memory>

#include <boost/filesystem.hpp>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "graphics-framework/viewport.hpp"
#include "graphics-framework/input_router.hpp"
#include "graphics-framework/geometry.hpp"
#include "graphics-framework/geometry_list.hpp"
#include "graphics-framework/window_handler.hpp"

using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::unique_ptr;
using std::make_unique;

using glm::vec3;

vector<vec3> test_data_lines = {
	glm::vec3(00.0, 00.0, 00.0),	//NOLINT: magic numbers ok, temp test code
	glm::vec3(200.0, 00.0, 00.0),	//NOLINT: magic numbers ok, temp test code
	glm::vec3(00.0, 200.0, 00.0),	//NOLINT: magic numbers ok, temp test code

	glm::vec3(00.0, 00.0, 00.0),	//NOLINT: magic numbers ok, temp test code
	glm::vec3(00.0, 00.0, 200.0),	//NOLINT: magic numbers ok, temp test code   
	glm::vec3(200.0, 00.0, 00.0),	//NOLINT: magic numbers ok, temp test code

	glm::vec3(00.0, 00.0, 00.0),	//NOLINT: magic numbers ok, temp test code
	glm::vec3(00.0, 200.0, 00.0),	//NOLINT: magic numbers ok, temp test code
	glm::vec3(00.0, 00.0, 200.0)	//NOLINT: magic numbers ok, temp test code	
};

auto old_time = std::chrono::steady_clock::now(), new_time = std::chrono::steady_clock::now();
double delta_t;	

shared_ptr<vector<shared_ptr<WindowHandler>>> window_handlers;
unique_ptr<InputRouter> input_router;
unique_ptr<GeometryList> master_geometry;

const int DEFAULT_WINDOW_WIDTH = 1024, DEFAULT_WINDOW_HEIGHT = 768;

void CreateRenderWindow(int width, int height, char* title, GLFWwindow* sharedWindow = nullptr) {
	 
	auto *glfw_window = glfwCreateWindow(width, height, title, nullptr, sharedWindow);
	window_handlers->push_back(std::make_shared<WindowHandler>(glfw_window));

	//Bind the viewport class pointer to the window within, so the callbacks can pass through to the non static
	//viewport callback handlers from the static inputRouter callback functions using the provided window.
	shared_ptr<WindowHandler> window_handler = window_handlers->back();
	glfwSetWindowUserPointer(glfw_window, window_handler.get());	

	glfwSetInputMode(glfw_window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetKeyCallback(glfw_window, input_router->KeyCallback);
	glfwSetMouseButtonCallback(glfw_window, input_router->MouseButtonCallback);
	glfwSetScrollCallback(glfw_window, input_router->ScrollCallback);
	glfwSetCursorPosCallback(glfw_window, input_router->CursorCallback);
	glfwSetWindowSizeCallback(glfw_window, input_router->WindowSizeCallback);
}

void ErrorCallback(int error, const char* description) {	//NOLINT: unnused callback params
	fprintf(stderr, "%s", description);
}

//TODO split into init() and mainLoop() functions.
int main(int argc, const char* argv[]) {	//NOLINT: unused params are standard for main()
    std::cout << "Launching Program" << std::endl;
	srand(time(nullptr));

	if( !glfwInit() ) {
		fprintf( stderr, "Failed to initialize GLFW\n" );
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //For MacOS compat, apparently
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	input_router = make_unique<InputRouter>();
	window_handlers = make_shared<vector<shared_ptr<WindowHandler>>>();
	master_geometry = make_unique<GeometryList>(window_handlers);

    CreateRenderWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, strdup("Render Window"));
	input_router->SetActiveWindowHandler((window_handlers->at(0)));
	//CreateRenderWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, strdup("Render Window"), window_handlers->at(0)->glfw_window);
	
	master_geometry->push_back(make_shared<Geometry>(test_data_lines, test_data_lines));

	glfwSetErrorCallback(ErrorCallback);

    while (true) {  //TODO: Write proper update & exit logic.
		old_time = new_time;
    	new_time = std::chrono::steady_clock::now();
		delta_t = std::chrono::duration_cast<std::chrono::milliseconds>(new_time - old_time).count();

		for (const auto& w : (*window_handlers)) {
			input_router->SetActiveWindowHandler(w);
        	w->Update(delta_t);

			//Update other events like input handling 
	    	glfwPollEvents();
		}

		auto geo_ptr = master_geometry->begin();

		while (geo_ptr != master_geometry->end()) {
			(*geo_ptr)->Update(delta_t);
			geo_ptr++;
		}
    }

	//glfwTerminate();
    return 0;
}

