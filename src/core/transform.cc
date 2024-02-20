#include "transform.hh"

Transform::Transform(Vector3 _position, Quaternion _rotation, Vector3 _scale) :
		position(_position),
		rotation(_rotation),
		scale(_scale) {}

void Transform::fpsRotate(const Vector2 _input) {
	rotation = Q_AA(AXIS_Y, _input.Y) * rotation;
	Vector3 localRight { rotate(V3_RIGHT, rotation) };
	rotation = Q_AA(localRight, _input.X) * rotation;
}

Vector3 Transform::getForward(void) {
	return rotate(V3_FORWARD, rotation);
}

Vector3 Transform::getRight(void) {
	return rotate(V3_RIGHT, rotation);
}

Vector3 Transform::getUp(void) {
	return rotate(V3_UP, rotation);
}

Vector3 Transform::getLookAt(void) {
	return position + rotate(V3_FORWARD, rotation);
}
