#include <stdio.h>

#include "../src/math/matrix4.h"
#include "../src/core/object.h"
#include "../src/math/vector3.h"

void test_object_position_set_get(void);
void test_matrix4(void);

int main(void) {
	// test_matrix4(); printf("\n");
	test_object_position_set_get(); printf("\n");
}

void test_object_position_set_get(void) {
	struct Object obj = OBJECT_CTOR;
	struct Vec3 v = { 1.0f, 2.0f, 3.0f };
	
	obj_set_position(&obj, &v);
	
	struct Vec3 pos = obj_get_position(&obj);
	printf("{ %2.2f, %2.2f, %2.2f }\n", pos.x, pos.y, pos.z);
	
	obj_set_position_xyz(&obj, 2.0f, 3.0f, 4.0f);
	
	pos = obj_get_position(&obj);
	printf("{ %2.2f, %2.2f, %2.2f }\n", pos.x, pos.y, pos.z);
}

void test_matrix4(void) {
	float a[16] = {
		1, 1, 1, 1,
		2, 2, 2, 2,
		3, 3, 3, 3,
		4, 4, 4, 4
	};
	float b[16] = {
		1, 2, 3, 4,
		1, 2, 3, 4,
		1, 2, 3, 4,
		1, 2, 3, 4
	};
	float target[16];
	
	mul_mtx_mtx(target, a, b);
	mtx_print(target);
}
