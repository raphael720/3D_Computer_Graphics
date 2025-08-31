#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "display.h"

#include "vector.h"
#include "array.h"
#include "mesh.h"

triangle_t* triangles_to_render = NULL;

vec3_t camera_position = { 0, 0, 0 };
float fov_factor = 640;

bool is_running = false;
int previous_frame_time = 0;


void setup(void) {
	// Initialize render mode
	render_method = RENDER_WIRE;
	cull_method = CULL_BACKFACE;

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
	load_obj_file_data("C:\\Users\\rapha\\OneDrive\\Desktop\\Raphael\\Projetos\\Pikuma\\assets\\cube.obj");
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
			if (event.key.keysym.sym == SDLK_1)
				render_method = RENDER_WIRE_VERTEX;
			if (event.key.keysym.sym == SDLK_2)
				render_method = RENDER_WIRE;
			if (event.key.keysym.sym == SDLK_3)
				render_method = RENDER_FILL_TRIANGLE;
			if (event.key.keysym.sym == SDLK_4)
				render_method = RENDER_FILL_TRIANGLE_WIRE;
			if (event.key.keysym.sym == SDLK_c)
				cull_method = CULL_BACKFACE;
			if (event.key.keysym.sym == SDLK_d)
				cull_method = CULL_NONE;
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
	mesh.rotation.y += 0.01;
	mesh.rotation.z += 0.01;

	// Loop for all triangles faces in the mesh.rotation
	vec3_t face_vertices[3];
	int num_faces = array_length(mesh.faces);
	for (int i = 0; i < num_faces; i++) {
		face_t mesh_face = mesh.faces[i];

		face_vertices[0] = mesh.vertices[mesh_face.a -1];
		face_vertices[1] = mesh.vertices[mesh_face.b -1];
		face_vertices[2] = mesh.vertices[mesh_face.c -1];

		vec3_t transformed_vertices[3];

		// Loop for all three vertices of the triangle and aply transformation
		for (int j = 0; j < 3; j++) {
			vec3_t transformed_vertex = face_vertices[j];

			transformed_vertex = vec3_rotate_x(&transformed_vertex, mesh.rotation.x);
			transformed_vertex = vec3_rotate_y(&transformed_vertex, mesh.rotation.y);
			transformed_vertex = vec3_rotate_z(&transformed_vertex, mesh.rotation.z);

			// Translate the vertex away from the camera
			transformed_vertex.z += 5;
			transformed_vertices[j] = transformed_vertex;
		}

		if (cull_method == CULL_BACKFACE) {
			// Checking the back-face culling
			vec3_t vector_a = transformed_vertices[0];
			vec3_t vector_b = transformed_vertices[1];
			vec3_t vector_c = transformed_vertices[2];

			vec3_t vector_ab = vec3_sub(vector_b, vector_a);
			vec3_t vector_ac = vec3_sub(vector_c, vector_a);
		
			// Cross product to find the normal vector
			vec3_t normal_vector = vec3_cross(vector_ab, vector_ac);

			//Normalize the normal vector
			vec3_normalize(&normal_vector);

			// The vector between the camera position and a traingle vertex
			vec3_t camera_ray = vec3_sub(camera_position, vector_a);

			float dot_normal_camera = vec3_dot(normal_vector, camera_ray);

			// Bypass the triangles that are looking away from the camera 
			if (dot_normal_camera < 0) continue;
		}


		// Loop for all three vertices to perform projection
		triangle_t projected_triangle;
		for (int j = 0; j < 3; j++) {
			vec2_t project_vectex = project(transformed_vertices[j]);

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
	//draw_grid();

	int num_triangles = array_length(triangles_to_render);
	for (int i = 0; i < num_triangles; i++) {
		triangle_t triangle = triangles_to_render[i];
		if (render_method == RENDER_WIRE_VERTEX) {
			draw_rect(triangle.points[0].x, triangle.points[0].y, 6, 6, RED);
			draw_rect(triangle.points[1].x, triangle.points[1].y, 6, 6, RED);
			draw_rect(triangle.points[2].x, triangle.points[2].y, 6, 6, RED);
		}

		if (render_method == RENDER_FILL_TRIANGLE || render_method == RENDER_FILL_TRIANGLE_WIRE) {
			draw_filled_triangle(triangle.points[0].x, triangle.points[0].y,
								triangle.points[1].x, triangle.points[1].y,
								triangle.points[2].x, triangle.points[2].y,
								GREEN);
		}

		if (render_method == RENDER_WIRE || render_method == RENDER_WIRE_VERTEX || render_method == RENDER_FILL_TRIANGLE_WIRE)
			draw_triangle(triangle.points[0], triangle.points[1], triangle.points[2], WHITE);
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