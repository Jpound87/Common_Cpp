#pragma once

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include "pch.h"
#include "framework.h"
#include <cmath>
#include <basetsd.h>


namespace Math_Angles
{

#pragma region Constants

    const UINT16 DEGREES_MAX = 360;
    const UINT16 DEGREES_MIN = 0;
    const double PI_DIVIDE_180 = M_PI / 180.0;

#pragma endregion

#pragma region Conversion

    //TODO: move to degree class in common or w/e
    static double dConvert_DegreesToRadians(double angleDegrees)
    {
        return  PI_DIVIDE_180 * angleDegrees;
    }

    static float fConvert_DegreesToRadians(float angleDegrees)
    {
        return static_cast<float>(dConvert_DegreesToRadians(static_cast<double>(angleDegrees)));
    }

    static UINT16 ReduceOrientation_Degrees(UINT16 orientationDegrees)
    {
        return orientationDegrees % DEGREES_MAX;
    }

#pragma endregion

#pragma region Validity

    static bool CheckValidOrientation_Degrees(UINT16 orientationDegrees)
    {
        return orientationDegrees < DEGREES_MAX && orientationDegrees > DEGREES_MIN;
    }

#pragma endregion

#undef _USE_MATH_DEFINES
}