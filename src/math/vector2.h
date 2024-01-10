#ifndef KE_VECTOR2_H
#define KE_VECTOR2_H

#include "math.h"

struct Vec2 {
	float x, y;
};

#define V2_CTOR(_x, _y) ((struct Vec2) { (_x), (_y) })

#define EQ_V2(_a, _b) (((_a).x == (_b).x && (_a).y == (_b).y))

#define ADD_V2_V2(_a, _b) ((struct Vec2) { (_a).x + (_b).x, (_a).y + (_b).y })
#define ADD_V2_F(_a, _b) ((struct Vec2) { (_a).x + (_b), (_a).y + (_b) })
#define SUB_V2_V2(_a, _b) ((struct Vec2) { (_a).x - (_b).x, (_a).y - (_b).y })
#define SUB_V2_F(_a, _b) ((struct Vec2) { (_a).x - (_b), (_a).y - (_b) })
#define MUL_V2_V2(_a, _b) ((struct Vec2) { (_a).x * (_b).x, (_a).y * (_b).y })
#define MUL_V2_F(_a, _b) ((struct Vec2) { (_a).x * (_b), (_a).y * (_b) })
#define DIV_V2_V2(_a, _b) ((struct Vec2) { (_a).x / (_b).x, (_a).y / (_b).y })
#define DIV_V2_F(_a, _b) ((struct Vec2) { (_a).x / (_b), (_a).y / (_b) })

#define DOT_V2(_a, _b) (((_a).x * (_b).x + (_a).y * (_b).y))
#define MAG_V2(_a) (sqrt(DOT_V2((_a), (_a))))
#define NORM_V2(_a) (MUL_V2_F((_a), 1.0f / MAG_V2(_a)))

#endif // KE_VECTOR2_H
