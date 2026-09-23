/***************************************************
 * FILENAME :		math.h
 * DESCRIPTION :
 * 		Contains many math related function definitions found in UYA.
 * NOTES :
 * AUTHOR :
 */

#ifndef _LIBRAC5_MATH_H_
#define _LIBRAC5_MATH_H_

#include "types.h"

//--------------------------------------------------------
#define MATH_PI                         ((float)3.141593)
#define MATH_TAU                        (MATH_PI * (float)2.0)
#define MATH_E                          ((float)2.7182818)
#define MATH_DT                         ((float)0.166666666)
#define MATH_RAD2DEG                    ((float)180 / MATH_PI)
#define MATH_DEG2RAD                    (MATH_PI / (float)180)

//--------------------------------------------------------
float cosf(float theta);
float sinf(float theta);
float acosf(float v);
float asinf(float v);
__LIBRAC5_INLINE__ float sqrtf(float f);
__LIBRAC5_INLINE__ float powf(float base, float exp);
__LIBRAC5_INLINE__ float fabsf(float f);
__LIBRAC5_INLINE__ float lerpf(float a, float b, float t);
__LIBRAC5_INLINE__ float signf(float a);
__LIBRAC5_INLINE__ float maxf(float a, float b);
__LIBRAC5_INLINE__ float minf(float a, float b);
__LIBRAC5_INLINE__ float floorf(float a);
__LIBRAC5_INLINE__ float lerpfAngle(float a, float b, float t);
__LIBRAC5_INLINE__ float clamp(float v, float min, float max);
__LIBRAC5_INLINE__ float clampAngle(float theta);
__LIBRAC5_INLINE__ double fmod(double x, double y);
float atan2f(float y, float x);
__LIBRAC5_INLINE__ float fastSubRots(float input0, float input1);
__LIBRAC5_INLINE__ float fastDiffRots(float input0, float input1);

#endif // _LIBRAC5_MATH_H_
