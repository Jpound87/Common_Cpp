#pragma once

#include "pch.h"

namespace HexCoordinates
{

#pragma region Enumerations
    enum HexOrientation : BOOL 
    { 
        PointTop, 
        FlatTop 
    };
#pragma endregion

#pragma region Constants
	const int CENTER_COORDINATE = 0;
#pragma endregion

#pragma region Cube

    /// <summary>
    /// Hex coordinate system stored in integer representation where the hex is considered a 3D cube.
    /// </summary>
    struct  HexCoordinate_iCube
    {
    public:
        HexCoordinate_iCube();
        HexCoordinate_iCube(const int x, const int y, const int z);

        const int X() const;
        const int Y() const;
        const int Z() const;
    };

    const HexCoordinate_iCube CENTER_ICUBE = HexCoordinate_iCube();

    /// <summary>
    /// Hex coordinate system stored in floating point representation where the hex is considered a 3D cube.
    /// </summary>
    struct  HexCoordinate_fCube
    {
    public:
        HexCoordinate_fCube();
        HexCoordinate_fCube(const float x, const float y, const float z);

        const float X() const;
        const float Y() const;
        const float Z() const;
    };

    const HexCoordinate_fCube CENTER_FCUBE = HexCoordinate_fCube();

#pragma endregion

#pragma region Axial

    /// <summary>
    /// Hex coordinate system stored in integer representation.
    /// </summary>
    struct  HexCoordinate_iAxial
    {
    public:
        HexCoordinate_iAxial();
        HexCoordinate_iAxial(const int q, const int r);
        HexCoordinate_iAxial(const int q, const int r, const int s);

        const int Q() const;
        const int R() const;
        const int S() const;
    };

    const HexCoordinate_iAxial CENTER_IAXIAL = HexCoordinate_iAxial();


    /// <summary>
    /// Hex coordinate system stored in floating point representation.
    /// </summary>
    struct  HexCoordinate_fAxial
    {
    public:
        HexCoordinate_fAxial();
        HexCoordinate_fAxial(const float q, const float r);
        HexCoordinate_fAxial(const float q, const float r, const float s);
        const float Q() const;
        const float R() const;
        const float S() const;
    };

    const HexCoordinate_fAxial CENTER_FAXIAL = HexCoordinate_fAxial();

#pragma endregion

#pragma region Conversion

    /// <summary>
    /// Converts the given 'Axial' coordinates to 'Cube' coordinates. 
    /// </summary>
    /// <param name="axialCoordinates">Coordinates to convert.</param>
    /// <returns>'Cube' coordinates</returns>
    static HexCoordinate_iCube Convert_AxialToCube(HexCoordinate_iAxial axialCoordinates)
    {
        return HexCoordinate_iCube(axialCoordinates.Q(), axialCoordinates.R(), axialCoordinates.S());
    }

    /// <summary>
    /// Converts the given 'Axial' coordinates to 'Cube' coordinates. 
    /// </summary>
    /// <param name="axialCoordinates">Coordinates to convert.</param>
    /// <returns>'Cube' coordinates</returns>
    static HexCoordinate_fCube Convert_AxialToCube(HexCoordinate_fAxial axialCoordinates)
    {
        return HexCoordinate_fCube(axialCoordinates.Q(), axialCoordinates.R(), axialCoordinates.S());
    }

    /// <summary>
    /// Converts the given 'Cube' coordinates to 'Axial' coordinates. 
    /// </summary>
    /// <param name="axialCoordinates">Coordinates to convert.</param>
    /// <returns>'Axial' coordinates</returns>
    static HexCoordinate_iAxial Convert_CubeToAxial(HexCoordinate_iCube cubeCoordinates)
    {
        return HexCoordinate_iAxial(cubeCoordinates.X(), cubeCoordinates.Y(), cubeCoordinates.Z());
    }


    /// <summary>
    /// Converts the given 'Cube' coordinates to 'Axial' coordinates. 
    /// </summary>
    /// <param name="axialCoordinates">Coordinates to convert.</param>
    /// <returns>'Axial' coordinates</returns>
    static HexCoordinate_fAxial Convert_CubeToAxial(HexCoordinate_fCube cubeCoordinates)
    {
        return HexCoordinate_fAxial(cubeCoordinates.X(), cubeCoordinates.Y(), cubeCoordinates.Z());
    }

#pragma endregion

}
