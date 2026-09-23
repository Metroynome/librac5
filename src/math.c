#include "math.h"
#include "interop.h"

//--------------------------------------------------------
float sinf(float x)
{
    // normalize to [-PI, PI]
    while (x >  MATH_PI) x -= MATH_TAU;
    while (x < -MATH_PI) x += MATH_TAU;

    float x2 = x * x;
    return x - (x  * x2) / 6.0f + (x  * x2 * x2) / 120.0f - (x  * x2 * x2 * x2) / 5040.0f + (x  * x2 * x2 * x2 * x2) / 362880.0f;
}

//--------------------------------------------------------
float cosf(float x)
{
    return sinf(x + MATH_PI / 2.0f);
}

//--------------------------------------------------------
// float acosf(float v)
// {
//     return (MATH_PI / 2) - asinf(v);
// }

//--------------------------------------------------------
float lerpf(float a, float b, float t)
{
    return (b-a)*t + a;
}

//--------------------------------------------------------
float fabsf(float f)
{
    union { float x; unsigned int i; } u = { f };
    u.i &= 0x7fffffff;
    return u.x;
}

//--------------------------------------------------------
float signf(float a)
{
    return (a < 0) ? -1 : 1;
}

//--------------------------------------------------------
float maxf(float a, float b)
{
    return (a < b) ? b : a;
}

//--------------------------------------------------------
float minf(float a, float b)
{
    return (a > b) ? b : a;
}

//--------------------------------------------------------
float lerpfAngle(float a, float b, float t)
{
    if (fabsf(a-b) < MATH_PI)
        return clampAngle(lerpf(a, b, t));
    else if (a < b)
        return clampAngle(lerpf(a, b - MATH_TAU, t));
    else
        return clampAngle(lerpf(a, b + MATH_TAU, t));
}

//--------------------------------------------------------
float clamp(float v, float min, float max)
{
    if (v < min)
        return min;
    if (v > max)
        return max;

    return v;
}

//--------------------------------------------------------
/*
float clampAngle(float theta)
{
  float f1 = (theta + MATH_PI) / MATH_TAU;
  return (f1 - (float)(int)f1) * MATH_TAU - MATH_PI;
}
*/

//--------------------------------------------------------

float clampAngle(float theta)
{
    if (theta > MATH_PI)
        theta -= MATH_TAU;
    else if (theta < -MATH_PI)
        theta += MATH_TAU;

    return theta;
}


//--------------------------------------------------------
float fastSubRots(float input0, float input1)
{
    input0 = input0 - input1;
    int i = input0 < -MATH_PI;
    if (MATH_PI <= input0)
        input0 = (input0 - MATH_PI) - MATH_PI;

    if (i)
        input0 = input0 + MATH_PI + MATH_PI;
    
    return input0;
}

float fastDiffRots(float input0, float input1)
{
    float diff = fabsf(input0 - input1);
    if (MATH_PI <= diff) 
        diff = MATH_TAU - diff;

    return diff;
}

//float sqrtf(float number) {
//    float x = number * 0.5f;
//    float y = number;
//    long i = *(long*)&y;           // evil float bit hack
//    i = 0x5f3759df - (i >> 1);     // what the hell?
//    y = *(float*)&i;
//    y = y * (1.5f - (x * y * y));  // 1st iteration
//    return number * y;
//}

float sqrtf(float x)
{
    if (x <= 0.0f) return 0.0f;
    float out;
    __asm__ volatile (
        "sqrt.s %0, %1\n"
         : "=f"(out)
        : "f"(x)
        );
    return out;
    }