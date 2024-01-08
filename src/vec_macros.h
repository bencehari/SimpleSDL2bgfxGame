#ifndef VEC_MATH_H
#define VEC_MATH_H

#define V3_CTOR(_x, _y, _z) ((struct Vec3) { (_x), (_y), (_z) })
#define V2_CTOR(_x, _y) ((struct Vec2) { (_x), (_y) })

#define EQ_V3(_a, _b) ((bool) ((_a).x == (_b).x && (_a).y == (_b).y && (_a).z == (_b).z))
#define EQ_V2(_a, _b) ((bool) ((_a).x == (_b).x && (_a).y == (_b).y))

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

#define DOT_V3(_a, _b) ((float) ((_a).x * (_b).x + (_a).y * (_b).y + (_a).z * (_b).z))
#define CROSS_V3(_a, _b) \
	((struct Vec3) { (_a).y * (_b).z - (_a).z * (_b).y, (_a).z * (_b).x - (_a).x * (_b).z, (_a).x * (_b).y - (_a).y * (_b).x })
	
#define DOT_V2(_a, _b) ((float) ((_a).x * (_b).x + (_a).y * (_b).y))


#endif // VEC_MATH_H
