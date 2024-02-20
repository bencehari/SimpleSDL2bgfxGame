#ifndef KE_TRANSFORM_HH
#define KE_TRANSFORM_HH

#include "../math/math.h"

/**
 * @brief Transform struct contains position, orientation and scale data.
*/
struct Transform {
	Vector3 position;
	Quaternion rotation;
	Vector3 scale;
	
	Transform(Vector3 _position, Quaternion _rotation, Vector3 _scale);

	/**
	 * @brief Rotates the transform based on input.
	 *
	 * Expects _input as rotation on X and Y axis in radians.
	 *
	 * @param _input Mouse input.
	*/
	void fpsRotate(const Vector2 _input);

	/**
	 * @brief Calculates forward vector.
	 *
	 * Calculates normalized forward (local Z+) Vector3 in world space.
	 *
	 * @return Normalized Vector3.
	*/
	Vector3 getForward(void);
	
	/**
	 * @brief Calculates right vector.
	 *
	 * Calculates normalized right (lcoal X+) Vector3 in world space.
	 *
	 * @return Normalized Vector3.
	*/
	Vector3 getRight(void);
	
	/**
	 * @brief Calculates up vector.
	 *
	 * Calculates normalized up (local Y+) Vector3 in world space.
	 *
	 * @return Normalized Vector3.
	*/
	Vector3 getUp(void);
	
	/**
	 * @brief Calculates look at vector.
	 *
	 * Calculates normalized forward (local Z+) Vector3 in world space
	 * from actual world position.
	 *
	 * @return Normalized Vector3.
	*/
	Vector3 getLookAt(void);
};

/**
 * @brief Creates a Transform with proper default values.
*/
#define TRANSFORM_NEW Transform(V3_ZERO, Q_IDENTITY, V3_ONE)

#endif // KE_TRANSFORM_HH
