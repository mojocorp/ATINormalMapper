#pragma once

#include <cmath>

// Value that's close enough to be called 0.0
#define EPSILON 1.0e-7

///////////////////////////////////////////////////////////////////////////////
// Normalize vector
///////////////////////////////////////////////////////////////////////////////
template<typename T>
void Normalize(T v[3])
{
    const T len = std::sqrt((v[0] * v[0]) + (v[1] * v[1]) + (v[2] * v[2]));
    if (len < EPSILON) {
        v[0] = 1.0;
        v[1] = 0.0;
        v[2] = 0.0;
    } else {
        v[0] = v[0] / len;
        v[1] = v[1] / len;
        v[2] = v[2] / len;
    }
}

////////////////////////////////////////////////////////////////////
// Multiply a 3x3 matrix with a 3 space vector (assuming w = 1), ignoring
// the last row of the matrix
////////////////////////////////////////////////////////////////////
inline void
ConvertFromTangentSpace(double* m, double* vec, double* result)
{
    double tmp[3];
    tmp[0] = vec[0] * m[0] + vec[1] * m[1] + vec[2] * m[2];
    tmp[1] = vec[0] * m[3] + vec[1] * m[4] + vec[2] * m[5];
    tmp[2] = vec[0] * m[6] + vec[1] * m[7] + vec[2] * m[8];

    result[0] = tmp[0];
    result[1] = tmp[1];
    result[2] = tmp[2];
}

////////////////////////////////////////////////////////////////////
// Multiply a 3x3 matrix with a 3 space vector (assuming w = 1), ignoring
// the last row of the matrix
////////////////////////////////////////////////////////////////////
inline void
ConvertToTangentSpace(double* m, double* vec, double* result)
{
    double tmp[3];
    tmp[0] = vec[0] * m[0] + vec[1] * m[3] + vec[2] * m[6];
    tmp[1] = vec[0] * m[1] + vec[1] * m[4] + vec[2] * m[7];
    tmp[2] = vec[0] * m[2] + vec[1] * m[5] + vec[2] * m[8];

    result[0] = tmp[0];
    result[1] = tmp[1];
    result[2] = tmp[2];
}
