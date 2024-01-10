#ifndef KE_OBJECT_H
#define KE_OBJECT_H

#include "../math/vector3.h"

#define OBJECT_CTOR ((struct Object) { {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f} })

struct Object {
	float transform[16];
};

void set_position(struct Object* _o, const struct Vec3* _v);
void set_position_xyz(struct Object* _o, float _x, float _y, float _z);
struct Vec3 get_position(const struct Object* _o);

#endif // KE_OBJECT_H
