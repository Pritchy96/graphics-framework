#include "graphics-framework/viewport.hpp"	
#include <glm/fwd.hpp>
#include <memory>
#include <vector>

using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::make_unique;

using glm::vec3;

const vector<vec3> AXIS_LINES = {
    vec3(0.0f, 0.0f, 0.0f),	//x
	vec3(500.0f, 0.0f, 0.0f),
	vec3(0.0f, 0.0f, 0.0f),	//y	
	vec3(0.0f, 500.0f, 0.0f), 
	vec3(0.0f, 0.0f, 0.0f),	//z
	vec3(0.0f, 0.0f, 500.0f)
};

const vector<vec3> AXIS_COLOURS = {
    vec3(1.0f, 0.0f, 0.0f),	//x
	vec3(1.0f, 0.0f, 0.0f),	
	vec3(0.0f, 1.0f, 0.0f), //y
	vec3(0.0f, 1.0f, 0.0f),
	vec3(0.0f, 0.0f, 1.0f),	//z
	vec3(0.0f, 0.0f, 1.0f)
};

void Viewport::SetFpsCounter(GLFWwindow* glfw_window, double deltaTime) {
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

Viewport::Viewport(GLFWwindow *window, glm::vec3 background_colour) {
    std::cout << "Initialised Viewport" << std::endl;
	glfw_window = window;

	glfwGetWindowSize(glfw_window, &width, &height);

	camera = new Camera(glm::vec3(100, 100, 100), glm::vec3(0, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f));
	arcball_camera = new Arcball(camera, width, height, .015f);
	input_handlers.push_back(arcball_camera);

	if( !glfw_window ) {
		fprintf(stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
	}

	//TODO: Move everything dependent on this (gl calls, shader loads etc) to an init() function so this context setting can be done 
	//in main.cpp
	glfwMakeContextCurrent(glfw_window);

	glClearColor(background_colour.r, background_colour.g, background_colour.b, 1.0f);
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glPointSize(2);

	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS); //Depth-testing interprets a smaller value as "closer"

	// Initialize GLEW
	glewExperimental = 1u; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
	}

	//TODO: file paths are currently relative to excution path, not main location.
	basic_shader = shader::LoadShaders((char*)"./shaders/basic.vertshader", (char*)"./shaders/basic.fragshader");

    shared_ptr<Geometry> render_axis = make_shared<Geometry>(AXIS_LINES, AXIS_COLOURS);
	shared_ptr<ViewportGrid> grid = make_shared<ViewportGrid>(80, 80, 40, 40, basic_shader);
	//TODO: better to have a GeoList in the viewport with just (this) in it's renderable list?
	geo_renderable_pairs.emplace_back(grid, make_unique<Renderable>(basic_shader, grid, GL_LINES));
	geo_renderable_pairs.emplace_back(render_axis, make_unique<Renderable>(basic_shader, render_axis, GL_LINES));
}

void Viewport::SetupTransformShader(GLuint transformShader) {
	t_shader = transformShader;
}

void Viewport::Update(double deltaTime) {
	SetFpsCounter(glfw_window, deltaTime);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto geo_renderable = geo_renderable_pairs.begin();

	while(geo_renderable != geo_renderable_pairs.end()) {
		//Geo is dead, nuke the map link
		if (geo_renderable->first->is_dead) {
			//iterator.erase gives the next item in the list.
			geo_renderable = geo_renderable_pairs.erase(geo_renderable);
			continue;
		}

		if (geo_renderable->second == nullptr) {
			//Renderable for geo doesn't exist, make one.
			//TODO: Some logic to choose a render type? (currently default to GL_TRIANGLES)
			geo_renderable->second = make_unique<Renderable>(basic_shader, geo_renderable->first, GL_TRIANGLES);
		}

		shared_ptr<Geometry> geometry = geo_renderable->first;
		shared_ptr<Renderable> renderable = geo_renderable->second;

		if (geometry->buffers_invalid) {
			renderable->valid_vao = false;
		}

		renderable->Draw(deltaTime, camera->GetProjectionMatrix(), camera->GetViewMatrix());
		++geo_renderable;
	}

	glfwSwapBuffers(glfw_window);
}

Viewport::~Viewport() {
	glfwDestroyWindow(glfw_window);
	glfwSetWindowUserPointer(glfw_window, nullptr);
	exit(EXIT_SUCCESS);
}

void Viewport::MouseButtonCallback( GLFWwindow* window, int button, int action, int mods ){
	for (InputHandler* i : input_handlers) {
    	i->MouseButtonCallback(window, button, action, mods);
	}
}
 
void Viewport::CursorCallback( GLFWwindow *window, double x, double y ) {
	for (InputHandler* i : input_handlers) {
    	i->CursorCallback(window, x, y);
	}
}

void Viewport::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	//Only allow zoom out if we're really close to the camera target
	if (glm::length(camera->position - camera->target) > 4.0f || yoffset > 0) {
		//TODO: properly implement a zoom function
		// camera->SetZoom(camera->GetZoom() + (yoffset*1.0f));
	}

	for (InputHandler* i : input_handlers) {
		i->ScrollCallback(window, xoffset, yoffset);
	}
}
	
void Viewport::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {	//NOLINT: unnused callback params
	switch (key) {
		case(GLFW_KEY_V) :
			if (action == GLFW_PRESS) {
				GLint mode[2];	//NOLINT: C style array required for glGetIntergerv
				glGetIntegerv(GL_POLYGON_MODE, mode);
				if (mode[0] == GL_LINE) {
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					fprintf( stdout, "Switching View to GL_FILL.\n" );
				} else {
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					fprintf( stdout, "Switching View to GL_LINE.\n" );
				}
			}
			break;

		case(GLFW_KEY_C) :
			if(action == GLFW_PRESS) {
				camera->SetProjection(!camera->ortho_not_perspective);
			}
			break;

		case(GLFW_KEY_G) :
			if(action == GLFW_PRESS) {
				//TODO: render special geo like the grid and axis seperately to avoid this janky by index referencing.
				geo_renderable_pairs.at(0).first->visible = !geo_renderable_pairs.at(0).first->visible;
			}
			break;

		default:
			break;
	} 
}

void Viewport::WindowSizeCallback(GLFWwindow* glfw_window, int width, int height) {
	this->height = height;
	this->width = width;
	glViewport(0, 0, width, height);

	for (InputHandler* i : input_handlers) {
		i->WindowSizeCallback(glfw_window, width, height);
	}
}