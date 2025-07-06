#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "mesh.h"
#include "array.h"


mesh_t mesh = {
	.vertices = NULL,
	.faces = NULL,
	.rotation = {.x = 0, .y = 0, .z = 0 }
};

vec3_t cube_vertices[N_CUBE_VERTICES] = {
	{.x = -1, .y = -1, .z = -1 },
	{.x = -1, .y = 1, .z = -1 },
	{.x = 1, .y = 1, .z = -1 },
	{.x = 1, .y = -1, .z = -1 },
	{.x = 1, .y = 1, .z = 1 },
	{.x = 1, .y = -1, .z = 1 },
	{.x = -1, .y = 1, .z = 1 },
	{.x = -1, .y = -1, .z = 1 }
};

face_t cube_faces[N_CUBE_FACES] = {
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

void load_cube_mesh_data(void) {
	for (int i = 0; i < N_CUBE_VERTICES; i++) {
		array_push(mesh.vertices, cube_vertices[i]);
	}
	for (int i = 0; i < N_CUBE_FACES; i++) {
		array_push(mesh.faces, cube_faces[i]);
	}
}

void load_obj_file_data(const char* filename) {
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    char line[1024];
    while (fgets(line, 1024, file)) {
      
        if (line[0] == 'v' && line[1] == ' ') {
            
            vec3_t vertex;
            sscanf(line, "v  %f %f %f", &vertex.x, &vertex.y, &vertex.z);
			array_push(mesh.vertices, vertex);
        }

        if (line[0] == 'f' && line[1] == ' ') {
         
			int vertices[3];
			int textures[3];
			int normals[3];
			sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", 
					&vertices[0], &textures[0], &normals[0],
					&vertices[1], &textures[1], &normals[1],
					&vertices[2], &textures[2], &normals[2]);
			
			face_t face = {.a = vertices[0], .b = vertices[1], .c = vertices[2]};
			array_push(mesh.faces, face);
        }
    }

    fclose(file);
}

