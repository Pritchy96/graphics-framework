#include "graphics-framework/viewport.hpp"	

using namespace std;
using namespace glm;

vector<vec3> axis_lines = {
    vec3(0.0f, 0.0f, 0.0f),	//x
	vec3(500.0f, 0.0f, 0.0f),
	vec3(0.0f, 0.0f, 0.0f),	//y	
	vec3(0.0f, 500.0f, 0.0f), 
	vec3(0.0f, 0.0f, 0.0f),	//z
	vec3(0.0f, 0.0f, 500.0f)
};

vector<vec3> axis_colours = {
    vec3(1.0f, 0.0f, 0.0f),	//x
	vec3(1.0f, 0.0f, 0.0f),	
	vec3(0.0f, 1.0f, 0.0f), //y
	vec3(0.0f, 1.0f, 0.0f),
	vec3(0.0f, 0.0f, 1.0f),	//z
	vec3(0.0f, 0.0f, 1.0f)
};

void Viewport::setFPSCounter(GLFWwindow* glfwWindow, double deltaTime) {
	timeElapsed += deltaTime;
	framesElapsed++;

	//If it's more than a quarter of a second, update fps.
	if (timeElapsed > 250) {
		double fps = (double)(framesElapsed / timeElapsed) * 1000;
		char tmp[128];
		//Write formatted data to tmp string.
		snprintf(tmp, sizeof(tmp)/sizeof(char), "Render Window @ %.2f FPS", fps);
		//Set glfwWindow title to string.
		glfwSetWindowTitle(glfwWindow, tmp);
		framesElapsed = 0;
		timeElapsed = 0;
	}
}

Viewport::Viewport(GLFWwindow *glfw_window, glm::vec3 background_colour) {
    cout << "Initialised Viewport" << endl;
	glfwWindow = glfw_window;

	glfwGetWindowSize(glfw_window, &width, &height);

	camera = new Camera(glm::vec3(100, 100, 100), glm::vec3(0, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f), width, height);
	arcballCamera = new Arcball(camera, width, height, .015f);
	inputHandlers.push_back(arcballCamera);

	if( !glfwWindow ) {
		fprintf(stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
	}

	//TODO: Move everything dependent on this (gl calls, shader loads etc) to an init() function so this context setting can be done 
	//in main.cpp
	glfwMakeContextCurrent(glfwWindow);

	glClearColor(background_colour.r, background_colour.g, background_colour.b, 0.01f);
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glPointSize(2);
	glLineWidth(4); //TODO: This doesn't work?

	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS); //Depth-testing interprets a smaller value as "closer"

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
	}
	
	//TODO: file paths are currently relative to excution path, not main location.
	basicShader = Shader::LoadShaders((char*)"./shaders/basic.vertshader", (char*)"./shaders/basic.fragshader");

    shared_ptr<Geometry> renderAxis = make_shared<Geometry>(axis_lines, axis_colours);
	shared_ptr<ViewportGrid> grid = make_shared<ViewportGrid>(80, 80, 40, 40, basicShader);
	//TODO: better to have a GeoList in the viewport with just (this) in it's renderable list?
	geoRenderablePairs.push_back(make_pair(grid, make_unique<Renderable>(basicShader, grid, GL_LINES)));
	geoRenderablePairs.push_back(make_pair(renderAxis, make_unique<Renderable>(basicShader, renderAxis, GL_LINES)));
}

void Viewport::setupTransformShader(GLuint transformShader) {
	tShader = transformShader;
}

void Viewport::update(float deltaTime) {
	setFPSCounter(glfwWindow, deltaTime);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto geoRenderable = geoRenderablePairs.begin();

	while(geoRenderable != geoRenderablePairs.end()) {
		//Geo is dead, nuke the map link
		if (geoRenderable->first->isDead) {
			//iterator.erase gives the next item in the list.
			geoRenderable = geoRenderablePairs.erase(geoRenderable);
			continue;
		}

		if (geoRenderable->second == NULL) {
			//Renderable for geo doesn't exist, make one.
			//TODO: Some logic to choose a render type? (currently default to GL_TRIANGLES)
			geoRenderable->second = make_unique<Renderable>(basicShader, geoRenderable->first, GL_TRIANGLES);
		}

		shared_ptr<Geometry> geometry = geoRenderable->first;
		shared_ptr<Renderable> renderable = geoRenderable->second;

		if (geometry->buffersInvalid) {
			renderable->validVAO = false;
		}

		renderable->Draw(deltaTime, camera->getProjectionMatrix(), camera->getViewMatrix());
		++geoRenderable;
	}

	glfwSwapBuffers(glfwWindow);
}

Viewport::~Viewport() {
	glfwDestroyWindow(glfwWindow);
	glfwSetWindowUserPointer(glfwWindow, NULL);
	exit(EXIT_SUCCESS);
}

void Viewport::mouseButtonCallback( GLFWwindow* window, int button, int action, int mods ){
	for (InputHandler* i : inputHandlers) {
    	i->mouseButtonCallback(window, button, action, mods);
	}
}
 
void Viewport::cursorCallback( GLFWwindow *window, double x, double y ) {
	for (InputHandler* i : inputHandlers) {
    	i->cursorCallback(window, x, y);
	}
}

void Viewport::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	//Only allow zoom out if we're really close to the camera target
	if (glm::length(camera->position - camera->target) > 4.0f || yoffset > 0)
		camera->SetZoom(camera->GetZoom() + (yoffset*1.0f));

	for (InputHandler* i : inputHandlers) {
		i->scrollCallback(window, xoffset, yoffset);
	}
}
	
void Viewport::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) {
		case(GLFW_KEY_V) :
			if (action == GLFW_PRESS) {
				GLint mode[2];
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
				camera->SetProjection(!camera->orthoNotPerspective);
			}
			break;

		case(GLFW_KEY_G) :
			if(action == GLFW_PRESS) {
				//TODO: render special geo like the grid and axis seperately to avoid this janky by index referencing.
				geoRenderablePairs.at(0).first->visible = !geoRenderablePairs.at(0).first->visible;
			}
			break;

		default:
			break;
	} 
}

void Viewport::windowSizeCallback(GLFWwindow* glfwWindow, int width, int height) {
	this->height = height;
	this->width = width;
	glViewport(0, 0, width, height);

	for (InputHandler* i : inputHandlers) {
		i->windowSizeCallback(glfwWindow, width, height);
	}
}