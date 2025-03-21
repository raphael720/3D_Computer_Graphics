#ifndef VECTOR_H_
#define VECTOR_H_

#include <math.h>

typedef struct {
	float x;
	float y;
} vec2_t;

typedef struct {
	float x;
	float y;
	float z;
} vec3_t;

vec3_t vec3_rotate_x(vec3_t* v, float angle);
vec3_t vec3_rotate_z(vec3_t* v, float angle);
vec3_t vec3_rotate_y(vec3_t* v, float angle);

#endif
