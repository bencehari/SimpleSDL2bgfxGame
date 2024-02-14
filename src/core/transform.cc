#include "transform.h"

void Transform::fpsRotate(const Vector2 _input) {
	rotation = Q_AA(AXIS_Y, _input.Y) * rotation;
	Vector3 localRight = rotate_v3_by_q(globalRight, rotation);
	rotation = Q_AA(localRight, _input.X) * rotation;
}

Vector3 Transform::getForward(void) {
	return rotate_v3_by_q(globalForward, rotation);
}

Vector3 Transform::getRight(void) {
	return rotate_v3_by_q(globalRight, rotation);
}

Vector3 Transform::getUp(void) {
	return rotate_v3_by_q(globalUp, rotation);
}

Vector3 Transform::getLookAt(void) {
	return position + rotate_v3_by_q(globalForward, rotation);
}
