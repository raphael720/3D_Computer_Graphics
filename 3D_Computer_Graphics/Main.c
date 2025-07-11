#include<stdio.h>
#include<stdlib.h>
#include "display.h"

#include "vector.h"
#include "array.h"
#include "mesh.h"

triangle_t* triangles_to_render = NULL;

vec3_t camera_position = { .x = 0, .y = 0, .z = -5 };
float fov_factor = 640;

bool is_running = false;
int previous_frame_time = 0;

void setup(void) {
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);

	// Create SDL texture that is used to display the color buffer
	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);

	//load_cube_mesh.rotation_data();
	load_obj_file_data("C:\\Users\\rapha\\OneDrive\\Desktop\\Raphael\\Projetos\\Pikuma\\assets\\f22.obj");
}

void process_input(void) {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
		case SDL_QUIT:
			is_running = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				is_running = false;
			break;
	}
}

vec2_t project(vec3_t point) {
	vec2_t projected_point = {
		.x = (fov_factor * point.x) / point.z,
		.y = (fov_factor * point.y) / point.z
	};

	return projected_point;
}

void update(void) {
	//while(!SDL_TICKS_PASSED(SDL_GetTicks(), previous_frame_time + FRAME_TARGET_TIME));
	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
		SDL_Delay(time_to_wait);
	previous_frame_time = SDL_GetTicks();

	// Initialize the triangles to render array
	triangles_to_render = NULL;

	mesh.rotation.x += 0.01;
	mesh.rotation.y += 0.0;
	mesh.rotation.z += 0.0;

	// Loop for all triangles faces in the mesh.rotation
	vec3_t face_vertices[3];
	int num_faces = array_length(mesh.faces);
	for (int i = 0; i < num_faces; i++) {
		face_t mesh_face = mesh.faces[i];

		face_vertices[0] = mesh.vertices[mesh_face.a -1];
		face_vertices[1] = mesh.vertices[mesh_face.b -1];
		face_vertices[2] = mesh.vertices[mesh_face.c -1];

		// Loop for all three vertices of the triangle and aply transformation
		triangle_t projected_triangle;
		for (int j = 0; j < 3; j++) {
			vec3_t transformed_vertex = face_vertices[j];

			transformed_vertex = vec3_rotate_x(&transformed_vertex, mesh.rotation.x);
			transformed_vertex = vec3_rotate_y(&transformed_vertex, mesh.rotation.y);
			transformed_vertex = vec3_rotate_z(&transformed_vertex, mesh.rotation.z);

			// Translate the vertex away from the camera
			transformed_vertex.z -= camera_position.z;

			vec2_t project_vectex = project(transformed_vertex);

			// scale the projected vertex to the middle of the screen
			project_vectex.x += window_width / 2;
			project_vectex.y += window_height / 2;

			projected_triangle.points[j] = project_vectex;
		}

		// Store the projected triangle
		//triangles_to_render[i] = projected_triangle;
		array_push(triangles_to_render, projected_triangle);
	}
}

void render(void) {
	
	int num_triangles = array_length(triangles_to_render);
	for (int i = 0; i < num_triangles; i++) {
		triangle_t triangle = triangles_to_render[i];
		draw_rect(triangle.points[0].x, triangle.points[0].y, 3, 3, 0xFFFFFF00);
		draw_rect(triangle.points[1].x, triangle.points[1].y, 3, 3, 0xFFFFFF00);
		draw_rect(triangle.points[2].x, triangle.points[2].y, 3, 3, 0xFFFFFF00);

		draw_triangle(triangle.points[0], triangle.points[1], triangle.points[2], 0xFF00FF00);
	}

	// clear triangles to render array
	array_free(triangles_to_render);

	render_color_buffer();
	clear_color_buffer(0xFF000000);

	SDL_RenderPresent(renderer);
}

void free_resources(void) {
	if (color_buffer) {
		free(color_buffer);
		color_buffer = NULL;
	}
	array_free(mesh.vertices);
	array_free(mesh.faces);
}

int main(int argc, char* args[]) {
	is_running = initialize_window();

	setup();

	while (is_running) {
		process_input();
		update();
		render();
	}

	destroy_window();
	free_resources();

	return 0;
}