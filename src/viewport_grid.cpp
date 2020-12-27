#include "graphics-framework/viewport_grid.hpp"

using glm::vec3;

ViewportGrid::ViewportGrid(int xLines, int yLines, float xSpacing, float ySpacing, GLuint shader) : Geometry() {
	SetupGrid(xLines, yLines, xSpacing, ySpacing);
}

void ViewportGrid::SetupGrid(int xLines, int yLines, float xSpacing, float ySpacing) {
	//Assume grid center is world coords x=0, y=0, z=0 for now
	vec3 center = vec3(0.0);

	float width = ((static_cast<float>(yLines)-1)*ySpacing);
	float height = ((static_cast<float>(xLines)-1)*xSpacing);
	float x_origin = center.x - (width/2);
	float y_origin = center.y - (height/2);

	vertexes.clear();

	//Columns
	for (int x = 0; x < xLines; x++) {
		vertexes.emplace_back(x_origin + (static_cast<float>(x)*xSpacing), y_origin, center.z);
		vertexes.emplace_back(x_origin + (static_cast<float>(x)*xSpacing), y_origin + height, center.z);
	}

	//Rows
	for (int y = 0; y < yLines; y++) {
		vertexes.emplace_back(x_origin, y_origin + (static_cast<float>(y) * ySpacing), center.z);
		vertexes.emplace_back(x_origin + width, y_origin + (static_cast<float>(y) * ySpacing), center.z);
	}

	GenerateFlatBuffers();
}