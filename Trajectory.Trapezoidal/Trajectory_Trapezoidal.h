#pragma once 

/***************************************************************************************************
* INTERFACE DEPENDENCIES
**************************************************************************************************/

#include <math.h>
#if !defined (_WINDOWS)
    #include <W1_Types/stdbool.h>
    #include <W1_Types/stdfloat.h>
#endif

/***************************************************************************************************
* PUBLIC MACRO DEFINITIONS
**************************************************************************************************/
#define DLL_EXPORT __declspec(dllexport)


/***************************************************************************************************
* PUBLIC TYPE DEFINITIONS
**************************************************************************************************/
typedef double_t trajectory_position_t;
typedef double_t trajectory_velocity_t;
typedef double_t trajectory_acceleration_t;
typedef double_t trajectory_time_t;
typedef int bool_t;
typedef int int32_t;


typedef struct
{
	bool_t complete;
	trajectory_time_t t;
	trajectory_acceleration_t a;
	trajectory_velocity_t v;
	trajectory_position_t s;
} s_trajectory_trapezoidalResult_t;

/***************************************************************************************************
* PUBLIC ATTRIBUTE DECLARATIONS
**************************************************************************************************/

/***************************************************************************************************
* PUBLIC OPERATION DECLARATIONS
**************************************************************************************************/
DLL_EXPORT void trajectory_trapezoidalInit(trajectory_position_t initialS);

DLL_EXPORT void trajectory_trapezoidalInitDelta(trajectory_position_t initialS, trajectory_time_t delT);

DLL_EXPORT void trajectory_trapezoidalTimeUpdate(trajectory_time_t timeUpdate);

DLL_EXPORT void trajectory_trapezoidalStart(
	trajectory_position_t finalS,
	trajectory_velocity_t v1,
	trajectory_acceleration_t a1,
	trajectory_acceleration_t a2,
	bool_t switchNow);

DLL_EXPORT void trajectory_trapezoidalQuickStop(
	trajectory_acceleration_t a2);

DLL_EXPORT s_trajectory_trapezoidalResult_t trajectory_trapezoidalGetResult();

