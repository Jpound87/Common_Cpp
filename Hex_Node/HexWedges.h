#include "pch.h"
#include "Math_Angles.h"
#include "HexCoordinates.h"

namespace HexWedges
{

#pragma region Enumerations
    
    enum WedgeOrientation : UINT8
    {
        deg0 = 0,
        deg30 = 1,
        deg60 = 2,
        deg90 = 3,
        deg120 = 4,
        deg150 = 5,
        deg180 = 6,
        deg210 = 7,
        deg240 = 8,
        deg270 = 9,
        deg300 = 10,
        deg330 = 11,
        deg360 = 12,
        INVALID = 13,
    };

#pragma endregion

#pragma region Constants

    const UINT8 HEX_SIDE_COUNT = 6;
    const UINT8 WEDGE_ORIENTATION_COUNT = 13;

    const UINT16 HexAngles_Degrees[WEDGE_ORIENTATION_COUNT] = 
    { 
        0, 
        30, 
        60, 
        90, 
        120, 
        150, 
        180, 
        210, 
        240, 
        270, 
        300, 
        330,
        360
    };

    const float HexAngles_Radians[WEDGE_ORIENTATION_COUNT]
    {
        Math_Angles::fConvert_DegreesToRadians(HexAngles_Degrees[0]),// here make 'common' degrees class, move the radians to the wedge 
        Math_Angles::fConvert_DegreesToRadians(HexAngles_Degrees[1]),
        Math_Angles::fConvert_DegreesToRadians(HexAngles_Degrees[2]),
        Math_Angles::fConvert_DegreesToRadians(HexAngles_Degrees[3]),
        Math_Angles::fConvert_DegreesToRadians(HexAngles_Degrees[4]),
        Math_Angles::fConvert_DegreesToRadians(HexAngles_Degrees[5]),
        Math_Angles::fConvert_DegreesToRadians(HexAngles_Degrees[6]),
        Math_Angles::fConvert_DegreesToRadians(HexAngles_Degrees[7]),
        Math_Angles::fConvert_DegreesToRadians(HexAngles_Degrees[8]),
        Math_Angles::fConvert_DegreesToRadians(HexAngles_Degrees[9]),
        Math_Angles::fConvert_DegreesToRadians(HexAngles_Degrees[10]),
        Math_Angles::fConvert_DegreesToRadians(HexAngles_Degrees[11])
    };

    const WedgeOrientation HexAngleIndicies_PointTop[HEX_SIDE_COUNT] 
    { 
        WedgeOrientation::deg0,
        WedgeOrientation::deg60,
        WedgeOrientation::deg120,
        WedgeOrientation::deg180,
        WedgeOrientation::deg240,
        WedgeOrientation::deg300
    };

    const WedgeOrientation HexAngleIndicies_FlatTop[HEX_SIDE_COUNT]
    { 
        WedgeOrientation::deg30,
        WedgeOrientation::deg90,
        WedgeOrientation::deg150,
        WedgeOrientation::deg210,
        WedgeOrientation::deg270,
        WedgeOrientation::deg330
    };

    const WedgeOrientation HexAngleIndicies_Invalid[HEX_SIDE_COUNT]
    { 
        WedgeOrientation::INVALID, 
        WedgeOrientation::INVALID,
        WedgeOrientation::INVALID,
        WedgeOrientation::INVALID,
        WedgeOrientation::INVALID,
        WedgeOrientation::INVALID
    };

    const UINT8 HexAngles_Invalid[HEX_SIDE_COUNT]
    { 
        HexAngles_Degrees[WedgeOrientation::INVALID],
        HexAngles_Degrees[WedgeOrientation::INVALID],
        HexAngles_Degrees[WedgeOrientation::INVALID],
        HexAngles_Degrees[WedgeOrientation::INVALID],
        HexAngles_Degrees[WedgeOrientation::INVALID],
        HexAngles_Degrees[WedgeOrientation::INVALID]
    };

#pragma endregion	

#pragma region Geometry

    /// <summary>
    /// Data structure intended to store the general characteristics of the geometry of the hex segments.
    /// Every hex will have 6 orientations, but each of these will have equivalent properties.
    /// </summary>
    struct Geometry_HexWedge
    {
    public:
        /// <summary>
        /// Wedge triangle is equalateral so all interior angles are the same.
        /// </summary>
        const UINT8 InteriorAngleDegrees = 60;

        Geometry_HexWedge();
        Geometry_HexWedge(WedgeOrientation wedgeOrientation);
        
        Geometry_HexWedge(const Geometry_HexWedge& hexWedgeGeometry);

        constexpr BOOLEAN Valid() const;
        constexpr WedgeOrientation Orientation() const;
        /// <summary>
        /// TODO define why constexpr
        /// </summary>
        /// <returns></returns>
        constexpr UINT16 Orientation_Degrees() const;

        /// <summary>
        /// TODO define why constexpr
        /// </summary>
        /// <returns></returns>
        constexpr float Orientation_Radians() const;
    };

    struct HexWedgeGeometryArray
    {
    public:
        HexWedgeGeometryArray();

        const Geometry_HexWedge GetWedgeAt(UINT8 index) const;
    };

    const Geometry_HexWedge HEX_WEDGE_GEOMETRY_INVALID = Geometry_HexWedge(HexWedges::WedgeOrientation::INVALID);

    const Geometry_HexWedge HEX_WEDGE_GEOMETRY_INVALID_ARRAY[HEX_SIDE_COUNT] =
    {
        HEX_WEDGE_GEOMETRY_INVALID,
        HEX_WEDGE_GEOMETRY_INVALID,
        HEX_WEDGE_GEOMETRY_INVALID,
        HEX_WEDGE_GEOMETRY_INVALID,
        HEX_WEDGE_GEOMETRY_INVALID,
        HEX_WEDGE_GEOMETRY_INVALID
    };

    const Geometry_HexWedge HEX_WEDGE_GEOMETRY_POINT_TOP_ARRAY[HEX_SIDE_COUNT] =
    {
        Geometry_HexWedge(HexAngleIndicies_PointTop[0]),
        Geometry_HexWedge(HexAngleIndicies_PointTop[1]),
        Geometry_HexWedge(HexAngleIndicies_PointTop[2]),
        Geometry_HexWedge(HexAngleIndicies_PointTop[3]),
        Geometry_HexWedge(HexAngleIndicies_PointTop[4]),
        Geometry_HexWedge(HexAngleIndicies_PointTop[5])
    };

    const Geometry_HexWedge HEX_WEDGE_GEOMETRY_FLAT_TOP_ARRAY[HEX_SIDE_COUNT] =
    {
        Geometry_HexWedge(HexAngleIndicies_FlatTop[0]),
        Geometry_HexWedge(HexAngleIndicies_FlatTop[1]),
        Geometry_HexWedge(HexAngleIndicies_FlatTop[2]),
        Geometry_HexWedge(HexAngleIndicies_FlatTop[3]),
        Geometry_HexWedge(HexAngleIndicies_FlatTop[4]),
        Geometry_HexWedge(HexAngleIndicies_FlatTop[5])
    };
#pragma endregion

}