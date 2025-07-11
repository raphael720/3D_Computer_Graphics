#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"

#define N_CUBE_VERTICES 8
#define N_CUBE_FACES 12
extern vec3_t cube_vertices[N_CUBE_VERTICES];
extern face_t cube_faces[N_CUBE_FACES];

typedef struct {
	vec3_t* vertices; // dynaminc Array of vertices
	face_t* faces; // dynaminc Array of faces
	vec3_t rotation; // rotation with x, y and z values of the mesh
} mesh_t;

extern mesh_t mesh;
void load_cube_mesh_data(void);
void load_obj_file_data(const char* filename);

#endif // MESH_H
