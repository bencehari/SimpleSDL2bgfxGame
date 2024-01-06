#include "../src/bx_math.h"

int main(void) {
	struct Vec3 v = vec3_ctor(1.0f, 2.0f, 3.0f);
	vec3_print(&v);
}