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
