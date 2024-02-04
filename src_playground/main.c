#include <stdio.h>
#include <time.h>

#include "../src/math/math.h"

Vector3 l_rotate_v3_by_q(const Vector3* _v3, const Quaternion* _q);
Vector3 l_rotate_v3_by_q_2(const Vector3* _v3, const Quaternion* _q);

void print_all(float _degree, const Quaternion* _q, const Vector3* _v3);
void print_degree(float _degree);
void print_quaternion(const Quaternion* _q);
void print_vector3(const Vector3* _v3);

void test(void);
void benchmark(void);

int main(void) {
	test();
	
	// benchmark();
}

void test(void) {
	Vector3 v = V3_FORWARD;
	Quaternion q = Q_IDENTITY;
	
	print_all(0.0f, &q, &v);
	
	for (int i = 1; i < 9; i++) {
		float angle = i * 45.0f;
		q = Q_MUL(Q_AA(AXIS_X, DEG_TO_RAD(angle)), Q_AA(AXIS_Y, DEG_TO_RAD(angle)));
		v = rotate_v3_by_q(&v, &q);
		
		print_all(angle, &q, &v);
	}
}

void benchmark(void) {
	int iter_num = 10000;
	
	Vector3 v = V3_FORWARD;
	Quaternion q = Q_IDENTITY;
	
	for (int aa = 0; aa < 4; aa++) {
		printf("iterations: %d\n", iter_num);
		
		time_t t = clock();
		
		v = V3_FORWARD;
		q = Q_IDENTITY;
		
		for (int a = 0; a < iter_num; a++) {
			for (int i = 1; i < 9; i++) {
				float angle = i * 45.0f;
				q = Q_AA(AXIS_Y, DEG_TO_RAD(angle));
				v = l_rotate_v3_by_q(&v, &q);
				
				// print_all(angle, &q, &v);
			}
		}
		
		t = clock() - t;
		printf("time (1): %fs\n", ((double)t)/CLOCKS_PER_SEC);
		
		v = V3_FORWARD;
		q = Q_IDENTITY;
		
		t = clock();
		
		for (int a = 0; a < iter_num; a++) {
			for (int i = 1; i < 9; i++) {
				float angle = i * 45.0f;
				q = Q_AA(AXIS_Y, DEG_TO_RAD(angle));
				v = l_rotate_v3_by_q_2(&v, &q);
				
				// print_all(angle, &q, &v);
			}
		}
		
		t = clock() - t;
		printf("time (2): %fs\n\n", ((double)t)/CLOCKS_PER_SEC);
		
		iter_num *= 10;
	}
}

Vector3 l_rotate_v3_by_q(const Vector3* _v3, const Quaternion* _q)
{
    Vector3 u = V3_NEW(_q->X, _q->Y, _q->Z);
    float s = _q->W;
	
	Vector3 cross = V3_CROSS(u, *_v3);

	return V3_ADD(
		V3_ADD(
			*_v3,
			V3_MUL_F(cross, s)
		),
		V3_MUL_F(V3_CROSS(u, cross), 2.0f)
	);
}

Vector3 l_rotate_v3_by_q_2(const Vector3* _v3, const Quaternion* _q)
{
    Vector3 u = V3_NEW(_q->X, _q->Y, _q->Z);
    float s = _q->W;
	
	float dot = V3_DOT(u, *_v3);
	Vector3 cross = V3_CROSS(u, *_v3);

	return V3_ADD(
		V3_ADD(
			V3_MUL_F(u, 2.0f * dot),
			V3_MUL_F(*_v3, (s * s - V3_DOT(u, u)))
		),
		V3_MUL_F(cross, 2.0f * s)
	);
}

void print_all(float _degree, const Quaternion* _q, const Vector3* _v3) {
	print_degree(_degree); printf("\t");
	print_quaternion(_q); printf("\t\t");
	print_vector3(_v3);
	printf("\n");
}

void print_degree(float _degree) {
	char c = -8;
	printf("%7.1f%c", _degree, c);
}

void print_quaternion(const Quaternion* _q) {
	printf("%7.3f %7.3f %7.3f %7.3f", _q->X, _q->Y, _q->Z, _q->W);
}

void print_vector3(const Vector3* _v3) {
	printf("%7.3f %7.3f %7.3f %7.3f", _v3->X, _v3->Y, _v3->Z, _v3->W);
}

/*
iter: 10000			01				02				03
---------------------------------------------------------------
time (1): 			0.016000s		0.015000s		0.016000s
time (2): 			0.015000s		0.016000s		0.009000s

iter: 100000
---------------------------------------------------------------
time (1): 			0.172000s		0.172000s		0.172000s
time (2): 			0.157000s		0.156000s		0.157000s

iter: 1000000
---------------------------------------------------------------
time (1): 			1.748000s		1.750000s		1.735000s
time (2): 			1.605000s		1.596000s		1.625000s

iter: 10000000
---------------------------------------------------------------
time (1): 			17.361000s		17.373000s		17.295000s
time (2): 			16.154000s		16.140000s		16.126000s
*/