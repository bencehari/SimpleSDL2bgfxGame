#include "math.h"

Vector3 rotate_v3_by_q(const Vector3* _v3, const Quaternion* _q) {
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
