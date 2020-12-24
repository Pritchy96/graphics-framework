#include "graphics-framework/viewport_grid.hpp"

using namespace glm;
using namespace std;

ViewportGrid::ViewportGrid(int xLines, int yLines, float xSpacing, float ySpacing, GLuint shader) : Geometry() {
	SetupGrid(xLines, yLines, xSpacing, ySpacing);
}

void ViewportGrid::SetupGrid(int xLines, int yLines, float xSpacing, float ySpacing) {
	//Assume grid center is world coords x=0, y=0, z=0 for now
	glm::vec3 center = glm::vec3(0.0);

	float width = ((yLines-1)*ySpacing),
		height = ((xLines-1)*xSpacing),
		xOrigin = center.x - (width/2),
		yOrigin = center.y - (height/2);

	vertexes.clear();

	//Columns
	for (int x = 0; x < xLines; x++) {
		vertexes.push_back(glm::vec3(xOrigin + (x*xSpacing), yOrigin, center.z));
		vertexes.push_back(glm::vec3(xOrigin + (x*xSpacing), yOrigin + height, center.z));
	}

	//Rows
	for (int y = 0; y < yLines; y++) {
		vertexes.push_back(glm::vec3(xOrigin, yOrigin + (y*ySpacing), center.z));
		vertexes.push_back(glm::vec3(xOrigin + width, yOrigin + (y*ySpacing), center.z));
	}

	GenerateFlatBuffers();
}