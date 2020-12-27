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

using namespace std;
using namespace boost;

vector<vec3> test_data_lines = {
	glm::vec3(00.0, 00.0, 00.0),
	glm::vec3(200.0, 00.0, 00.0),
	glm::vec3(00.0, 200.0, 00.0),

	glm::vec3(00.0, 00.0, 00.0),
	glm::vec3(00.0, 00.0, 200.0),
	glm::vec3(200.0, 00.0, 00.0),

	glm::vec3(00.0, 00.0, 00.0),
	glm::vec3(00.0, 200.0, 00.0),
	glm::vec3(00.0, 00.0, 200.0)
};

auto old_time = chrono::steady_clock::now(), new_time = chrono::steady_clock::now();
double delta_t;	

shared_ptr<vector<shared_ptr<Viewport>>> renderers;
unique_ptr<InputRouter> input_router;
unique_ptr<GeometryList> master_geometry;

void CreateRenderWindow(int width, int height, char* title, glm::vec3 backgroundCol, GLFWwindow* sharedWindow = NULL) {
	 
	renderers->push_back(make_shared<Viewport>(glfwCreateWindow(width, height, title, NULL, sharedWindow), backgroundCol));

	//Bind the viewport class pointer to the window within, so the callbacks can pass through to the non static
	//viewport callback handlers from the static inputRouter callback functions using the provided window.
	shared_ptr<Viewport> viewport = renderers->back();
	glfwSetWindowUserPointer(viewport->glfw_window, viewport.get());	

	glfwSetInputMode(renderers->back()->glfw_window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetKeyCallback(renderers->back()->glfw_window, input_router->KeyCallback);
	glfwSetMouseButtonCallback(renderers->back()->glfw_window, input_router->MouseButtonCallback);
	glfwSetScrollCallback(renderers->back()->glfw_window, input_router->ScrollCallback);
	glfwSetCursorPosCallback(renderers->back()->glfw_window, input_router->CursorCallback);
	glfwSetWindowSizeCallback(renderers->back()->glfw_window, input_router->WindowSizeCallback);
}

void ErrorCallback(int error, const char* description) {
	fprintf(stderr, "%s", description);
}

//TODO split into init() and mainLoop() functions.
int main(int argc, const char* argv[]) {
    cout << "Launching Program" << endl;
	srand(time(NULL));

	if( !glfwInit() ) {
		fprintf( stderr, "Failed to initialize GLFW\n" );
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //For MacOS compat, apparently
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	input_router = make_unique<InputRouter>();
	renderers = make_shared<vector<shared_ptr<Viewport>>>();
	master_geometry = make_unique<GeometryList>(renderers);

    CreateRenderWindow(1024, 768, (char*)"Render Window", glm::vec3(0.7f, 0.7f, 0.7f));
	input_router->SetActiveViewport(renderers->at(0));
	CreateRenderWindow(1024, 768, (char*)"Render Window", glm::vec3(0.4f, 0.4f, 0.4f), renderers->at(0)->glfw_window);
	
	master_geometry->push_back(make_shared<Geometry>(test_data_lines, test_data_lines));

	renderers->at(1)->camera->position = glm::vec3(0, 0, 10);

	glfwSetErrorCallback(ErrorCallback);

    while (true) {  //TODO: Write proper update & exit logic.
		old_time = new_time;
    	new_time = chrono::steady_clock::now();
		delta_t = chrono::duration_cast<chrono::milliseconds>(new_time - old_time).count();

		for (auto v : (*renderers)) {
			input_router->SetActiveViewport(v);
        	v->Update(delta_t);

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

