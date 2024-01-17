#ifndef KE_HMM_EXT_H
#define KE_HMM_EXT_H

#define ZERO_V3 (HMM_V3(0.0f, 0.0f, 0.0f))
#define UP_V3 (HMM_V3(0.0f, 1.0f, 0.0f))
#define DOWN_V3 (HMM_V3(0.0f, -1.0f, 0.0f))
#define LEFT_V3 (HMM_V3(-1.0f, 0.0f, 0.0f))
#define RIGHT_V3 (HMM_V3(-1.0f, 0.0f, 0.0f))
#define FORWARD_V3 (HMM_V3(0.0f, 0.0f, 1.0f))
#define BACK_V3 (HMM_V3(0.0f, 0.0f, -1.0f))

#define AXIS_X (RIGHT_V3)
#define AXIS_Y (UP_V3)
#define AXIS_Z (FORWARD_V3)

#endif // KE_HMM_EXT_H
