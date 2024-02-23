#pragma once

#include "pch.h"
#include "HexCoordinates.h"
#include "HexWedges.h"
#include "Math_Constants.h"


namespace HexGeometries
{

#pragma region Constants
	const UINT8 INVALID_MARGIN = 0;
#pragma endregion


#pragma region HexOrientation


#pragma endregion


#pragma region Evaluation

 /*   static HexCoordinates_iCube Evaluate_Size(HexCoordinates_iAxial axialCoordinates)
    {
        return HexCoordinates_iCube(axialCoordinates.Q(), axialCoordinates.R(), axialCoordinates.S());
    }*/

#pragma endregion

#pragma region Margins

	/// <summary>
	/// Calculates the horizontal distance between Hex centriods. 
	/// </summary>
	/// <returns>The horizontal margin of the hex with the given size.</returns>
	static double CalculateMargin_Horizontal(double size, HexCoordinates::HexOrientation orientation)
	{
		switch (orientation)
		{
		case HexCoordinates::HexOrientation::PointTop:
			return size * 2;// Don't use in dHexGeometry, use step.
		case HexCoordinates::HexOrientation::FlatTop:
			return MathConstants::SQRT3 * size;
		default:
			return INVALID_MARGIN;
		}
	}

	/// <summary>
	/// Calculates the horizontal distance between Hex centriods. This function is specialized for dHexGeometry
	/// to aviod differences between the margin and step.
	/// </summary>
	/// <returns>The horizontal margin of the hex with the given step and size.</returns>
	static double CalculateMargin_Horizontal(double step, double size, HexCoordinates::HexOrientation orientation)
	{
		switch (orientation)
		{
		case HexCoordinates::HexOrientation::PointTop:
			return step;
		case HexCoordinates::HexOrientation::FlatTop:
			return MathConstants::SQRT3 * size;
		default:
			return INVALID_MARGIN;
		}
	}

	/// <summary>
	/// Calculates the vertical distance between Hex centriods. 
	/// </summary>
	/// <returns>The vertical margin of the hex with the given size.</returns>
	static double CalculateMargin_Vertical(double size, HexCoordinates::HexOrientation orientation)
	{
		switch (orientation)
		{
		case HexCoordinates::HexOrientation::PointTop:
			return MathConstants::SQRT3 * size;
		case HexCoordinates::HexOrientation::FlatTop:
			return size * 2;// Don't use in dHexGeometry, use step.
		default:
			return INVALID_MARGIN;
		}
	}

	/// <summary>
	/// Calculates the vertical distance between Hex centriods. This function is specialized for dHexGeometry
	/// to aviod differences between the margin and step.
	/// </summary>
	/// <returns>The vertical margin of the hex with the given step and size.</returns>
	static double CalculateMargin_Vertical(double step, double size, HexCoordinates::HexOrientation orientation)
	{
		switch (orientation)
		{
		case HexCoordinates::HexOrientation::PointTop:
			return MathConstants::SQRT3 * size;
		case HexCoordinates::HexOrientation::FlatTop:
			return step;
		default:
			return INVALID_MARGIN;
		}
	}

#pragma endregion

#pragma region Wedges
	

#pragma endregion


}