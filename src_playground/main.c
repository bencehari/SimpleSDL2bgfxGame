#include <stdio.h>

#include "../src/bx_math.h"

int main(void) {
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
