///JLB

#include "pch.h"
#include "HexCoordinates.h"

using namespace HexCoordinates;

#ifdef __cplusplus    // If used by C++ code, 
extern "C"
{          // we need to export the C interface
#endif

#pragma region Cube

    /// <summary>
    /// Hex coordinate system stored in integer representation where the hex is considered a 3D cube.
    /// </summary>
    struct  HexCoordinate_iCube
    {
    private:
        const int _x, _y, _z;
    public:
        HexCoordinate_iCube() : _x(CENTER_COORDINATE), _y(CENTER_COORDINATE), _z(CENTER_COORDINATE)
        {

        }
        HexCoordinate_iCube(const int x, const int y, const int z) : _x(x), _y(y), _z(z)
        {

        }
        const int X() const
        {
            return _x;
        }
        const int Y() const
        {
            return _y;
        }
        const int Z() const
        {
            return _z;
        }
    };

    /// <summary>
    /// Hex coordinate system stored in floating point representation where the hex is considered a 3D cube.
    /// </summary>
    struct  HexCoordinate_fCube
    {
    private:
        const float _x, _y, _z;
    public:
        HexCoordinate_fCube() : _x(CENTER_COORDINATE), _y(CENTER_COORDINATE), _z(CENTER_COORDINATE)
        {

        }
        HexCoordinate_fCube(const float x, const float y, const float z) : _x(x), _y(y), _z(z)
        {

        }
        const float X() const
        {
            return _x;
        }
        const float Y() const
        {
            return _y;
        }
        const float Z() const
        {
            return _z;
        }
    };

#pragma endregion

#pragma region Axial

    /// <summary>
    /// Hex coordinate system stored in integer representation.
    /// </summary>
    struct  HexCoordinate_iAxial
    {
    private:
        const int _q, _r, _s;
    public:
        HexCoordinate_iAxial() : _q(CENTER_COORDINATE), _r(CENTER_COORDINATE), _s(CENTER_COORDINATE)
        {

        }
        HexCoordinate_iAxial(const int q, const int r) : _q(q), _r(r), _s(-q - r)
        {

        }
        HexCoordinate_iAxial(const int q, const int r, const int s) : _q(q), _r(r), _s(s)
        {

        }
        const int Q() const
        {
            return _q;
        }
        const int R() const
        {
            return _r;
        }
        const int S() const
        {
            return _s;
        }
    };


    /// <summary>
    /// Hex coordinate system stored in floating point representation.
    /// </summary>
    struct  HexCoordinate_fAxial
    {
    private:
        float _q, _r, _s;
    public:
        HexCoordinate_fAxial() : _q(CENTER_COORDINATE), _r(CENTER_COORDINATE), _s(CENTER_COORDINATE)
        {

        }
        HexCoordinate_fAxial(const float q, const float r) : _q(q), _r(r), _s(-q - r)
        {

        }
        HexCoordinate_fAxial(const float q, const float r, const float s) : _q(q), _r(r), _s(s)
        {

        }
        const float Q() const
        {
            return _q;
        }
        const float R() const
        {
            return _r;
        }
        const float S() const
        {
            return _s;
        }
    };

#pragma endregion



#ifdef __cplusplus
}
#endif

