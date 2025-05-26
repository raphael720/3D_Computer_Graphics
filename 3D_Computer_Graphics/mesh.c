#include "mesh.h"

vec3_t mesh_vertices[N_MESH_VERTICES] = {
	{.x = -1, .y = -1, .z = -1 },
	{.x = -1, .y = 1, .z = -1 },
	{.x = 1, .y = 1, .z = -1 },
	{.x = 1, .y = -1, .z = -1 },
	{.x = 1, .y = 1, .z = 1 },
	{.x = 1, .y = -1, .z = 1 },
	{.x = -1, .y = 1, .z = 1 },
	{.x = -1, .y = -1, .z = 1 }
};

face_t mesh_faces[N_MESH_FACES] = {
	// front
	{.a = 0, .b = 1, .c = 2},
	{.a = 0, .b = 2, .c = 3},
	// right
	{.a = 3, .b = 2, .c = 4},
	{.a = 3, .b = 4, .c = 5},
	// back
	{.a = 5, .b = 4, .c = 6},
	{.a = 5, .b = 6, .c = 7},
	// left
	{.a = 7, .b = 6, .c = 1},
	{.a = 7, .b = 1, .c = 0},
	// top
	{.a = 1, .b = 6, .c = 4},
	{.a = 1, .b = 4, .c = 2},
	// bottom
	{.a = 5, .b = 7, .c = 0},
	{.a = 5, .b = 0, .c = 3}
};