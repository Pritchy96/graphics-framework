#include "geometry.hpp"

using namespace glm;
using namespace std;

Geometry::Geometry(vector<glm::vec3> vert_data) {
	vertexes = vert_data;
	colours = vert_data;
	GenerateFlatBuffers();
}

Geometry::Geometry(vector<glm::vec3> vert_data, vector<glm::vec3> colour_data) {
	vertexes = vert_data;
	colours = colour_data;
	GenerateFlatBuffers();
}

void Geometry::Update(float deltaT) {
	if (buffersInvalid) {
		GenerateFlatBuffers();
	}
}

int Geometry::GenerateFlatBuffers() {
	for (vector<glm::vec3>::const_iterator point = vertexes.begin(); point!=vertexes.end(); ++point) {
		flatVerts.push_back(point->x);
		flatVerts.push_back(point->y);
		flatVerts.push_back(point->z);
	}

	for (vector<glm::vec3>::const_iterator colour = colours.begin(); colour!=colours.end(); ++colour) {
		flatCols.push_back(colour->x);
		flatCols.push_back(colour->y);
		flatCols.push_back(colour->z);
	}

	buffersInvalid = false;
	return vertexes.size();
}