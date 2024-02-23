
/***************************************************************************************************
 *   \brief      Trapezoidal trajectory.
 *   \details    Trapezoidal blended motion trajectory generator.
 *   \file       trajectory_trapezoidal.c 
 ***************************************************************************************************/

#pragma region Dependencies

#include "trajectory_trapezoidal.h"

#pragma endregion 

#pragma region Enums

typedef enum
{
	trajectory_TRAPEZOIDAL_SEGMENT1,
	trajectory_TRAPEZOIDAL_SEGMENT2,
	trajectory_TRAPEZOIDAL_SEGMENT3,
	trajectory_TRAPEZOIDAL_SEGMENT_COMPLETE
} e_trajectory_trapezoidalSegment_t;

typedef enum
{
	trajectory_MOTION_NORMAL,
	trajectory_MOTION_STOP,
	trajectory_MOTION_STOP_AND_REVERSE
} e_trajectory_motionMode_t;

#pragma endregion 

#pragma region Structs

typedef struct
{
	trajectory_velocity_t velocityEps;
	trajectory_time_t delT;
	int32_t direction;
	trajectory_time_t t;
	e_trajectory_trapezoidalSegment_t segment;
	e_trajectory_motionMode_t motionMode;
	trajectory_velocity_t V1;
	trajectory_acceleration_t A1;
	trajectory_acceleration_t A2;
	trajectory_velocity_t directionV1;
	trajectory_acceleration_t directionA1;
	trajectory_acceleration_t directionA2;
	trajectory_velocity_t currentV;
	trajectory_velocity_t currentDelV;
	trajectory_velocity_t nextDelV;
	trajectory_position_t currentS;
	trajectory_position_t currentDelS;
	trajectory_position_t nextDelS;
	trajectory_position_t finalS;
	trajectory_position_t reverseFinalS;
	bool_t complete;
} s_trajectory_trapezoidalHandler_t;

#pragma endregion 

#pragma region Static

static s_trajectory_trapezoidalHandler_t trapezoidalHandler;
static volatile bool_t pointBreak = false;

#pragma endregion 

#pragma region Functions 

static bool_t trajectory_velocityOutsideLimitCheck(int32_t direction, trajectory_velocity_t velocity, trajectory_velocity_t velocityLimit)
{
	if (direction > 0)
	{
		if (velocity > velocityLimit)
		{
			return true;
		}
	}
	else
	{
		if (velocity < velocityLimit)
		{
			return true;
		}
	}
}

static bool_t trajectory_velocityInsideOrEqualLimitCheck(int32_t direction, trajectory_velocity_t velocity, trajectory_velocity_t velocityLimit)
{
	if (direction > 0)
	{
		if (velocity <= velocityLimit)
		{
			return true;
		}
	}
	else
	{
		if (velocity >= velocityLimit)
		{
			return true;
		}
	}
	return false;
}

static bool_t trajectory_positionOutsideLimitCheck(int32_t direction, trajectory_position_t position, trajectory_position_t positionLimit)
{
	if (direction > 0)
	{
		if (position > positionLimit)
		{
			return true;
		}
	}
	else
	{
		if (position < positionLimit)
		{
			return true;
		}
	}
	return false;
}

static bool_t trajectory_positionOutsideOrEqualLimitCheck(int32_t direction, trajectory_position_t position, trajectory_position_t positionLimit)
{
	if (direction > 0)
	{
		if (position >= positionLimit)
		{
			return true;
		}
	}
	else
	{
		if (position <= positionLimit)
		{
			return true;
		}
	}
	return false;
}

static bool_t trajectory_positionInsideOrEqualLimitCheck(int32_t direction, trajectory_position_t position, trajectory_position_t positionLimit)
{
	if (direction > 0)
	{
		if (position <= positionLimit)
		{
			return true;
		}
	}
	else
	{
		if (position >= positionLimit)
		{
			return true;
		}
	}
	return false;
}

DLL_EXPORT void trajectory_trapezoidalInit(trajectory_position_t initialS)
{
	trajectory_trapezoidalInitDelta(initialS, 5.00E-04);
}

DLL_EXPORT void trajectory_trapezoidalInitDelta(trajectory_position_t initialS, trajectory_time_t delT)
{
	trapezoidalHandler.velocityEps = 1.00E-10; // Epsilon value to check velocity equals
	trapezoidalHandler.delT = delT;
	trapezoidalHandler.direction = 1;
	trapezoidalHandler.t = -trapezoidalHandler.delT;
	trapezoidalHandler.segment = trajectory_TRAPEZOIDAL_SEGMENT_COMPLETE;
	trapezoidalHandler.motionMode = trajectory_MOTION_NORMAL;
	trapezoidalHandler.V1 = 65536;
	trapezoidalHandler.A1 = 6553600;
	trapezoidalHandler.A2 = 6553600;
	trapezoidalHandler.directionV1 = 65536;
	trapezoidalHandler.directionA1 = 6553600;
	trapezoidalHandler.directionA2 = 6553600;
	trapezoidalHandler.currentV = 0;
	trapezoidalHandler.currentDelV = 0;
	trapezoidalHandler.currentS = initialS;
	trapezoidalHandler.currentDelS = 0;
	trapezoidalHandler.nextDelS = 0;
	trapezoidalHandler.finalS = 0;
	trapezoidalHandler.reverseFinalS = 0;
	trapezoidalHandler.complete = true;
}

DLL_EXPORT void trajectory_trapezoidalTimeUpdate(trajectory_time_t timeUpdate)
{
	trapezoidalHandler.t = timeUpdate;
}

DLL_EXPORT void trajectory_trapezoidalStart(trajectory_position_t finalS, trajectory_velocity_t v1,	trajectory_acceleration_t a1, trajectory_acceleration_t a2, bool_t switchNow)
{
	int32_t nextDirection;
	if (trapezoidalHandler.complete || switchNow)
	{
		nextDirection = finalS < trapezoidalHandler.currentS ? -1 : 1;
		trapezoidalHandler.motionMode = trajectory_MOTION_NORMAL;
		trapezoidalHandler.segment = trajectory_TRAPEZOIDAL_SEGMENT1;
		trapezoidalHandler.direction = nextDirection;
		trapezoidalHandler.finalS = finalS;
		trapezoidalHandler.V1 = v1;
		trapezoidalHandler.A1 = a1;
		trapezoidalHandler.A2 = a2;
		trapezoidalHandler.directionV1 = trapezoidalHandler.direction * v1;
		trapezoidalHandler.directionA1 = trapezoidalHandler.direction * a1;
		trapezoidalHandler.directionA2 = trapezoidalHandler.direction * a2;
		trapezoidalHandler.complete = false;
		switchNow = false;
	}
	else
	{
		trajectory_position_t nextS = trapezoidalHandler.currentS + trapezoidalHandler.currentDelS;
		trajectory_velocity_t nextV = trapezoidalHandler.currentV + trapezoidalHandler.currentDelV;
		trajectory_position_t nextDelS3 = 0.5 * (nextV * (nextV / (trapezoidalHandler.direction * a2)));
		nextDirection = finalS < trapezoidalHandler.currentS ? trapezoidalHandler.direction : -trapezoidalHandler.direction;
		if (nextDirection == trapezoidalHandler.direction)
		{
			trapezoidalHandler.motionMode = trajectory_MOTION_NORMAL;
			if (v1 > fabs(trapezoidalHandler.currentV))
			{
				trapezoidalHandler.segment = trajectory_TRAPEZOIDAL_SEGMENT1;
			}
			else if (v1 < fabs(trapezoidalHandler.currentV))
			{
				trapezoidalHandler.segment = trajectory_TRAPEZOIDAL_SEGMENT3;
			}
			else
			{
				if (trapezoidalHandler.segment == trajectory_TRAPEZOIDAL_SEGMENT3)
				{
					trapezoidalHandler.segment = trajectory_TRAPEZOIDAL_SEGMENT1;
				}
			}
			trapezoidalHandler.finalS = finalS;
			trapezoidalHandler.V1 = v1;
			trapezoidalHandler.A1 = a1;
			trapezoidalHandler.A2 = a2;
			trapezoidalHandler.directionV1 = trapezoidalHandler.direction * v1;
			trapezoidalHandler.directionA1 = trapezoidalHandler.direction * a1;
			trapezoidalHandler.directionA2 = trapezoidalHandler.direction * a2;
			trapezoidalHandler.complete = false;
		}
		else
		{
			// Reverse move needed
			trapezoidalHandler.motionMode = trajectory_MOTION_STOP_AND_REVERSE;
			trapezoidalHandler.reverseFinalS = finalS;
			trapezoidalHandler.V1 = v1;
			trapezoidalHandler.A1 = a1;
			trapezoidalHandler.A2 = a2;
			trapezoidalHandler.directionV1 = trapezoidalHandler.direction * v1;
			trapezoidalHandler.directionA1 = trapezoidalHandler.direction * a1;
			trapezoidalHandler.directionA2 = trapezoidalHandler.direction * a2;
			trapezoidalHandler.complete = false;
			trapezoidalHandler.segment = trajectory_TRAPEZOIDAL_SEGMENT3;
		}
	}
}

DLL_EXPORT void trajectory_trapezoidalQuickStop(trajectory_acceleration_t a2)
{
	trapezoidalHandler.motionMode = trajectory_MOTION_STOP;
	trapezoidalHandler.A2 = a2;
	trapezoidalHandler.directionA2 = trapezoidalHandler.direction * a2;

	trapezoidalHandler.segment = trajectory_TRAPEZOIDAL_SEGMENT3;
}

DLL_EXPORT s_trajectory_trapezoidalResult_t trajectory_trapezoidalGetResult()
{
	s_trajectory_trapezoidalResult_t trapezoidalResult{};

    trapezoidalHandler.t += trapezoidalHandler.delT;

    trapezoidalResult.complete = false;
    trapezoidalResult.t = trapezoidalHandler.t;
    trapezoidalResult.a = 0;
	trapezoidalResult.v = 0;
	trapezoidalResult.s = trapezoidalHandler.currentS;

	if (trapezoidalHandler.finalS == trapezoidalHandler.currentS || trapezoidalHandler.complete)
	{
		trapezoidalHandler.complete = true;
	}
	else
	{
		if (trapezoidalHandler.segment == trajectory_TRAPEZOIDAL_SEGMENT1)
		{
			trapezoidalHandler.currentV += trapezoidalHandler.currentDelV;
			trapezoidalHandler.currentS += trapezoidalHandler.currentDelS;

			trapezoidalHandler.nextDelV = trapezoidalHandler.directionA1 * trapezoidalHandler.delT;
			trapezoidalHandler.nextDelS = (trapezoidalHandler.currentV + 0.5 * trapezoidalHandler.nextDelV) * trapezoidalHandler.delT;

			trajectory_velocity_t nextV = trapezoidalHandler.currentV + trapezoidalHandler.nextDelV;
			trajectory_position_t nextS = trapezoidalHandler.currentS + trapezoidalHandler.nextDelS;

			trajectory_position_t delS3 = 0.5 * (nextV * (nextV / trapezoidalHandler.directionA2));

			if (trajectory_positionOutsideLimitCheck(
				trapezoidalHandler.direction,
				(nextS + delS3),
				trapezoidalHandler.finalS))
			{
				trapezoidalHandler.segment = trajectory_TRAPEZOIDAL_SEGMENT3;

				trajectory_velocity_t lowV1 = sqrt((trapezoidalHandler.directionA2 * (2.0 * trapezoidalHandler.directionA2 * (trapezoidalHandler.finalS - trapezoidalHandler.currentS) +
					(trapezoidalHandler.currentV * trapezoidalHandler.currentV))) /
					(trapezoidalHandler.directionA1 + trapezoidalHandler.directionA2));
				lowV1 *= trapezoidalHandler.direction;

				trajectory_time_t delT1 = (lowV1 - trapezoidalHandler.currentV) / trapezoidalHandler.directionA1;
				trajectory_velocity_t nextV = lowV1 - trapezoidalHandler.directionA2 * (trapezoidalHandler.delT - delT1);

				trapezoidalHandler.nextDelV = nextV - trapezoidalHandler.currentV;
				trapezoidalHandler.nextDelS = 0.5 * (trapezoidalHandler.currentV + lowV1) * delT1 + (lowV1 + (0.5 * trapezoidalHandler.nextDelV)) * (trapezoidalHandler.delT - delT1);
			} 
			else if (trajectory_velocityOutsideLimitCheck(
					trapezoidalHandler.direction,
					nextV,
					trapezoidalHandler.directionV1) &&
				trajectory_velocityInsideOrEqualLimitCheck(
					trapezoidalHandler.direction,
					trapezoidalHandler.currentV,
					trapezoidalHandler.directionV1))
			{
				trajectory_time_t delT1 = (trapezoidalHandler.directionV1 - trapezoidalHandler.currentV) / trapezoidalHandler.directionA1;
				trajectory_time_t delT3 = trapezoidalHandler.directionV1 / trapezoidalHandler.directionA2;
				trajectory_position_t delS3 = 0.5 * trapezoidalHandler.directionV1 * delT3;
				trajectory_time_t delT2 = ((trapezoidalHandler.finalS - trapezoidalHandler.currentS) - 0.5 * (trapezoidalHandler.directionV1 + trapezoidalHandler.currentV) * delT1 - delS3) / trapezoidalHandler.directionV1;
				
				if ((delT1 + delT2) < trapezoidalHandler.delT)
				{
					trapezoidalHandler.segment = trajectory_TRAPEZOIDAL_SEGMENT3;
					trapezoidalHandler.nextDelV = trapezoidalHandler.directionV1 - trapezoidalHandler.currentV - trapezoidalHandler.directionA2 * (trapezoidalHandler.delT - (delT1 + delT2));
					trapezoidalHandler.nextDelS = 0.5 * (trapezoidalHandler.directionV1 + trapezoidalHandler.currentV) * delT1 + trapezoidalHandler.directionV1 * delT2 + (trapezoidalHandler.directionV1 + 0.5 * trapezoidalHandler.nextDelV) * (trapezoidalHandler.delT - (delT1 + delT2));
				}
				else
				{
					trapezoidalHandler.segment = trajectory_TRAPEZOIDAL_SEGMENT2;
					trapezoidalHandler.nextDelV = trapezoidalHandler.directionV1 - trapezoidalHandler.currentV;
					trapezoidalHandler.nextDelS = 0.5 * (trapezoidalHandler.currentV + trapezoidalHandler.directionV1) * delT1 + trapezoidalHandler.directionV1 * (trapezoidalHandler.delT - delT1);
				}
			}

			trapezoidalHandler.currentDelV = trapezoidalHandler.nextDelV;
			trapezoidalHandler.currentDelS = trapezoidalHandler.nextDelS;

			trapezoidalResult.a = trapezoidalHandler.directionA1;
			trapezoidalResult.v = trapezoidalHandler.currentV;
			trapezoidalResult.s = trapezoidalHandler.currentS;
		}
		else if (trapezoidalHandler.segment == trajectory_TRAPEZOIDAL_SEGMENT2)
		{
			trapezoidalHandler.currentV += trapezoidalHandler.currentDelV;
			trapezoidalHandler.currentS += trapezoidalHandler.currentDelS;

			trapezoidalHandler.nextDelV = 0;
			trapezoidalHandler.nextDelS = trapezoidalHandler.currentV * trapezoidalHandler.delT;

			trajectory_velocity_t nextV = trapezoidalHandler.currentV + trapezoidalHandler.nextDelV;
			trajectory_position_t nextS = trapezoidalHandler.currentS + trapezoidalHandler.nextDelS;

			trajectory_position_t delS3 = 0.5 * (nextV * (nextV / trapezoidalHandler.directionA2));

			if (trajectory_positionOutsideLimitCheck(
				trapezoidalHandler.direction,
				(nextS + delS3),
				trapezoidalHandler.finalS))
			{
				trajectory_time_t delT3 = trapezoidalHandler.currentV / trapezoidalHandler.directionA2;
				delS3 = 0.5 * trapezoidalHandler.currentV * delT3;
				trajectory_time_t delT2 = (trapezoidalHandler.finalS - trapezoidalHandler.currentS - delS3) / trapezoidalHandler.currentV;

				trapezoidalHandler.segment = trajectory_TRAPEZOIDAL_SEGMENT3;
				trapezoidalHandler.nextDelV = -trapezoidalHandler.directionA2 * (trapezoidalHandler.delT - delT2);
				trapezoidalHandler.nextDelS = trapezoidalHandler.currentV * delT2 + (trapezoidalHandler.currentV + 0.5 * trapezoidalHandler.nextDelV) * (trapezoidalHandler.delT - delT2);
			}

			trapezoidalHandler.currentDelV = trapezoidalHandler.nextDelV;
			trapezoidalHandler.currentDelS = trapezoidalHandler.nextDelS;

			trapezoidalResult.a = 0;
			trapezoidalResult.v = trapezoidalHandler.currentV;
			trapezoidalResult.s = trapezoidalHandler.currentS;
		}
		else if (trapezoidalHandler.segment == trajectory_TRAPEZOIDAL_SEGMENT3)
		{
			trapezoidalHandler.currentV += trapezoidalHandler.currentDelV;
			trapezoidalHandler.currentS += trapezoidalHandler.currentDelS;

			trapezoidalHandler.nextDelV = -trapezoidalHandler.directionA2 * trapezoidalHandler.delT;
			trapezoidalHandler.nextDelS = (trapezoidalHandler.currentV + 0.5 * trapezoidalHandler.nextDelV) * trapezoidalHandler.delT;

			trajectory_velocity_t nextV = trapezoidalHandler.currentV + trapezoidalHandler.nextDelV;
			trajectory_position_t nextS = trapezoidalHandler.currentS + trapezoidalHandler.nextDelS;

			switch (trapezoidalHandler.motionMode)
			{
				case trajectory_MOTION_NORMAL:
				{
					if (trajectory_positionOutsideLimitCheck(
						trapezoidalHandler.direction,
						nextS,
						trapezoidalHandler.finalS) ||
						trajectory_velocityInsideOrEqualLimitCheck(
							trapezoidalHandler.direction,
							nextV,
							0.0))
					{
						trajectory_position_t delS3 = trapezoidalHandler.finalS - trapezoidalHandler.currentS; // 0.5 * trapezoidalHandler.currentV * delT3;

						trapezoidalHandler.segment = trajectory_TRAPEZOIDAL_SEGMENT_COMPLETE;
						trapezoidalHandler.nextDelV = -trapezoidalHandler.currentV;
						trapezoidalHandler.nextDelS = delS3;
					}
					else if (trajectory_velocityInsideOrEqualLimitCheck(
						trapezoidalHandler.direction,
						nextV,
						trapezoidalHandler.directionV1) &&
						trajectory_velocityOutsideLimitCheck(
							trapezoidalHandler.direction,
							trapezoidalHandler.currentV,
							trapezoidalHandler.directionV1))
					{
						trajectory_time_t delT31 = -(trapezoidalHandler.directionV1 - trapezoidalHandler.currentV) / trapezoidalHandler.directionA2;
						trajectory_time_t delT3 = trapezoidalHandler.directionV1 / trapezoidalHandler.directionA2;
						trajectory_time_t delT2 = (trapezoidalHandler.finalS - trapezoidalHandler.currentS - 0.5 * (trapezoidalHandler.currentV + trapezoidalHandler.directionV1) * delT31 - 0.5 * trapezoidalHandler.directionV1 * delT3) / trapezoidalHandler.directionV1;
						trajectory_time_t delT32 = (trapezoidalHandler.delT - (delT31 + delT2));

						if ((delT31 + delT2) < trapezoidalHandler.delT)
						{
							trapezoidalHandler.segment = trajectory_TRAPEZOIDAL_SEGMENT3; // remain in same segment
							trapezoidalHandler.nextDelV = (trapezoidalHandler.currentV - trapezoidalHandler.directionV1) + (trapezoidalHandler.directionV1 - trapezoidalHandler.directionA2 * delT32);
							trapezoidalHandler.nextDelS = 0.5 * (trapezoidalHandler.currentV + trapezoidalHandler.directionV1) * delT31 + trapezoidalHandler.directionV1 * delT2 + (trapezoidalHandler.directionV1 - 0.5 * trapezoidalHandler.directionA2 * delT32) * delT32;
						}
						else
						{
							trapezoidalHandler.segment = trajectory_TRAPEZOIDAL_SEGMENT2;
							trapezoidalHandler.nextDelV = trapezoidalHandler.directionV1 - trapezoidalHandler.currentV;
							trapezoidalHandler.nextDelS = 0.5 * (trapezoidalHandler.currentV + trapezoidalHandler.directionV1) * delT31 + trapezoidalHandler.directionV1 * (trapezoidalHandler.delT - delT31);
						}
					}
					break;
				}

				case trajectory_MOTION_STOP:
				{
					if (trajectory_velocityOutsideLimitCheck(
						-trapezoidalHandler.direction,
						nextV,
						0.0))
					{
						trajectory_time_t delT3 = trapezoidalHandler.currentV / trapezoidalHandler.directionA2;
						trajectory_position_t delS3 = 0.5 * trapezoidalHandler.currentV * delT3;

						trapezoidalHandler.segment = trajectory_TRAPEZOIDAL_SEGMENT_COMPLETE;
						trapezoidalHandler.nextDelV = -trapezoidalHandler.currentV;
						trapezoidalHandler.nextDelS = delS3;
					}
					break;
				}

				case trajectory_MOTION_STOP_AND_REVERSE:
				{
					if (trajectory_velocityOutsideLimitCheck(
						-trapezoidalHandler.direction,
						nextV,
						0.0))
					{
						trajectory_time_t delT3 = trapezoidalHandler.currentV / trapezoidalHandler.directionA2;
						trajectory_position_t delS3 = 0.5 * trapezoidalHandler.currentV * delT3;

						trapezoidalHandler.segment = trajectory_TRAPEZOIDAL_SEGMENT_COMPLETE;
						trapezoidalHandler.nextDelV = -trapezoidalHandler.currentV;
						trapezoidalHandler.nextDelS = delS3;

						trapezoidalHandler.motionMode = trajectory_MOTION_NORMAL;
						trapezoidalHandler.segment = trajectory_TRAPEZOIDAL_SEGMENT1;
						trapezoidalHandler.finalS = trapezoidalHandler.reverseFinalS;
						trapezoidalHandler.reverseFinalS = 0;
						trapezoidalHandler.direction *= -1;
						trapezoidalHandler.directionV1 = trapezoidalHandler.direction * trapezoidalHandler.V1;
						trapezoidalHandler.directionA1 = trapezoidalHandler.direction * trapezoidalHandler.A1;
						trapezoidalHandler.directionA2 = trapezoidalHandler.direction * trapezoidalHandler.A2;
					}
					break;
				}
			}

			trapezoidalHandler.currentDelV = trapezoidalHandler.nextDelV;
			trapezoidalHandler.currentDelS = trapezoidalHandler.nextDelS;

			trapezoidalResult.a = -trapezoidalHandler.directionA2;
			trapezoidalResult.v = trapezoidalHandler.currentV;
			trapezoidalResult.s = trapezoidalHandler.currentS;
		}
		else if (trapezoidalHandler.segment == trajectory_TRAPEZOIDAL_SEGMENT_COMPLETE)
		{
			trapezoidalHandler.currentV += trapezoidalHandler.currentDelV;
			trapezoidalHandler.currentS += trapezoidalHandler.currentDelS;

			trapezoidalHandler.nextDelV = 0;
			trapezoidalHandler.nextDelS = 0;

			trapezoidalHandler.currentDelV = trapezoidalHandler.nextDelV;
			trapezoidalHandler.currentDelS = trapezoidalHandler.nextDelS;

			trapezoidalHandler.currentV = 0;
			trapezoidalHandler.currentDelV = 0;
			trapezoidalHandler.currentDelS = 0;

			trapezoidalResult.a = 0;
			trapezoidalResult.v = 0;
			trapezoidalResult.s = trapezoidalHandler.currentS;
			trapezoidalHandler.complete = true;
		}
	}

	trapezoidalResult.complete = trapezoidalHandler.complete;

	return trapezoidalResult;
}
#pragma endregion 