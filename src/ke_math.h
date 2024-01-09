#ifndef KE_MATH_H
#define KE_MATH_H

#define _USE_MATH_DEFINES
#include <math.h>

#define ZERO_V3 ((struct Vec3) { 0.0f, 0.0f, 0.0f })
#define UP_V3 ((struct Vec3) { 0.0f, 1.0f, 0.0f })
#define DOWN_V3 ((struct Vec3) { 0.0f, -1.0f, 0.0f })
#define LEFT_V3 ((struct Vec3) { -1.0f, 0.0f, 0.0f })
#define RIGHT_V3 ((struct Vec3) { -1.0f, 0.0f, 0.0f })
#define FORWARD_V3 ((struct Vec3) { 0.0f, 0.0f, 1.0f })
#define BACK_V3 ((struct Vec3) { 0.0f, 0.0f, -1.0f })

#define V3_CTOR(_x, _y, _z) ((struct Vec3) { (_x), (_y), (_z) })
#define V2_CTOR(_x, _y) ((struct Vec2) { (_x), (_y) })

#define EQ_V3(_a, _b) (((_a).x == (_b).x && (_a).y == (_b).y && (_a).z == (_b).z))
#define EQ_V2(_a, _b) (((_a).x == (_b).x && (_a).y == (_b).y))

#define ADD_V3_V3(_a, _b) ((struct Vec3) { (_a).x + (_b).x, (_a).y + (_b).y, (_a).z + (_b).z })
#define ADD_V3_F(_a, _b) ((struct Vec3) { (_a).x + (_b), (_a).y + (_b), (_a).z + (_b) })
#define SUB_V3_V3(_a, _b) ((struct Vec3) { (_a).x - (_b).x, (_a).y - (_b).y, (_a).z - (_b).z })
#define SUB_V3_F(_a, _b) ((struct Vec3) { (_a).x - (_b), (_a).y - (_b), (_a).z - (_b) })
#define MUL_V3_V3(_a, _b) ((struct Vec3) { (_a).x * (_b).x, (_a).y * (_b).y, (_a).z * (_b).z })
#define MUL_V3_F(_a, _b) ((struct Vec3) { (_a).x * (_b), (_a).y * (_b), (_a).z * (_b) })
#define DIV_V3_V3(_a, _b) ((struct Vec3) { (_a).x / (_b).x, (_a).y / (_b).y, (_a).z / (_b).z })
#define DIV_V3_F(_a, _b) ((struct Vec3) { (_a).x / (_b), (_a).y / (_b), (_a).z / (_b) })

#define ADD_V2_V2(_a, _b) ((struct Vec2) { (_a).x + (_b).x, (_a).y + (_b).y })
#define ADD_V2_F(_a, _b) ((struct Vec2) { (_a).x + (_b), (_a).y + (_b) })
#define SUB_V2_V2(_a, _b) ((struct Vec2) { (_a).x - (_b).x, (_a).y - (_b).y })
#define SUB_V2_F(_a, _b) ((struct Vec2) { (_a).x - (_b), (_a).y - (_b) })
#define MUL_V2_V2(_a, _b) ((struct Vec2) { (_a).x * (_b).x, (_a).y * (_b).y })
#define MUL_V2_F(_a, _b) ((struct Vec2) { (_a).x * (_b), (_a).y * (_b) })
#define DIV_V2_V2(_a, _b) ((struct Vec2) { (_a).x / (_b).x, (_a).y / (_b).y })
#define DIV_V2_F(_a, _b) ((struct Vec2) { (_a).x / (_b), (_a).y / (_b) })

#define DOT_V3(_a, _b) (((_a).x * (_b).x + (_a).y * (_b).y + (_a).z * (_b).z))
#define CROSS_V3(_a, _b) \
	((struct Vec3) { (_a).y * (_b).z - (_a).z * (_b).y, (_a).z * (_b).x - (_a).x * (_b).z, (_a).x * (_b).y - (_a).y * (_b).x })
#define MAG_V3(_a) (sqrt(DOT_V3((_a), (_a))))
#define NORM_V3(_a) (MUL_V3_F((_a), 1.0f / MAG_V3(_a)))
	
#define DOT_V2(_a, _b) (((_a).x * (_b).x + (_a).y * (_b).y))
#define MAG_V2(_a) (sqrt(DOT_V2((_a), (_a))))
#define NORM_V2(_a) (MUL_V2_F((_a), 1.0f / MAG_V2(_a)))

#define RAD(_a) ((_a) * (M_PI / 180.0f))

#endif // KE_MATH_H
