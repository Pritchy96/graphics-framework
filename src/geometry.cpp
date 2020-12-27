#include "graphics-framework/geometry.hpp"

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
	if (buffers_invalid) {
		GenerateFlatBuffers();
	}
}

int Geometry::GenerateFlatBuffers() {
	for (vector<glm::vec3>::const_iterator point = vertexes.begin(); point!=vertexes.end(); ++point) {
		flat_verts.push_back(point->x);
		flat_verts.push_back(point->y);
		flat_verts.push_back(point->z);
	}

	for (vector<glm::vec3>::const_iterator colour = colours.begin(); colour!=colours.end(); ++colour) {
		flat_cols.push_back(colour->x);
		flat_cols.push_back(colour->y);
		flat_cols.push_back(colour->z);
	}

	buffers_invalid = false;
	return vertexes.size();
}