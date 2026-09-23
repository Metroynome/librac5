#include "math_helpers.h"
#include "math3d.h"

u128 internal_vectorReflect(u128 input, u128 normal)
{
    union { u128 bits; float v[4]; } a = {input}, n = {normal};
    float scale = 2 * (a.v[0]*n.v[0] + a.v[1]*n.v[1] + a.v[2]*n.v[2]);
    a.v[0] -= scale*n.v[0];
    a.v[1] -= scale*n.v[1];
    a.v[2] -= scale*n.v[2];
    return a.bits;
}

void matrix_unit(MATRIX output)
{
    int i;
    for (i = 0; i < 16; ++i)
        output[i] = (i % 5 == 0) ? 1.0f : 0.0f;
}
