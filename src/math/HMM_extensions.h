#ifndef KE_HMM_EXTENSIONS_H
#define KE_HMM_EXTENSIONS_H

#include "HandmadeMath.h"

// Q = quaternion
// in case of unit Q, the Q's conjugate (Q*) equals the inverse (Q^-1) of that Q
static inline HMM_Quat HMM_ConjQ(HMM_Quat Left)
{
    HMM_Quat Result;
    Result.X = -Left.X;
    Result.Y = -Left.Y;
    Result.Z = -Left.Z;
    Result.W = Left.W;

    return Result;
}

#define CONJ_Q(_a) (HMM_ConjQ((_a)))

#endif // KE_HMM_EXTENSIONS_H
