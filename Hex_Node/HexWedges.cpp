///


#include "HexWedges.h"
#include "Math_Angles.h"

using namespace HexWedges;
using namespace Math_Angles;

#ifdef __cplusplus    // If used by C++ code, 
extern "C"
{          // we need to export the C interface
#endif

#pragma region Geometry

	/// <summary>
	/// Data structure intended to store the general characteristics of the geometry of the hex segments.
	/// Every hex will have 6 orientations, but each of these will have equivalent properties.
	/// </summary>
	struct Geometry_HexWedge
	{
	private:
		const BOOLEAN _valid;
		/// <summary>
		/// Orientation in degrees off normal.
		/// </summary>
		const WedgeOrientation _wedgeOrientation;
	
	public:
		const UINT8 InteriorAngleDegrees = 60;

		Geometry_HexWedge() :
			_wedgeOrientation(WedgeOrientation::INVALID),
			_valid(false)
		{
			//TODO: Validity unit test
		}

		Geometry_HexWedge(WedgeOrientation wedgeOrientation) :
			_wedgeOrientation(wedgeOrientation),
			_valid(wedgeOrientation != WedgeOrientation::INVALID)
		{
			//TODO: Validity unit test
		}

		Geometry_HexWedge(const Geometry_HexWedge& hexWedgeGeometry) :
			_wedgeOrientation(hexWedgeGeometry.Orientation()),
			_valid(hexWedgeGeometry.Orientation() != WedgeOrientation::INVALID)
		{

		}

	/*	Geometry_HexWedge& operator=(const Geometry_HexWedge other) noexcept
		{
			if (this == &other)
			{
				return *this;
			}
			delete &_wedgeOrientation;
			_wedgeOrientation = std::exchange(other._wedgeOrientation, nullptr);

		}*/

		/// <summary>
		/// TODO define why constexpr
		/// </summary>
		/// <returns></returns>
		constexpr BOOLEAN Valid() const
		{
			return _valid;
		}

		/// <summary>
		/// TODO define why constexpr
		/// </summary>
		/// <returns></returns>
		constexpr WedgeOrientation Orientation() const
		{
			return _wedgeOrientation;
		}

		/// <summary>
		/// TODO define why constexpr
		/// </summary>
		/// <returns></returns>
		constexpr UINT16 Orientation_Degrees() const
		{
			if (!_valid)
			{
				return DEGREES_MAX;// Degrees max is not a listed angle (same as 0 ofc). 
			}
			return HexAngles_Degrees[_wedgeOrientation];
		}

		/// <summary>
		/// TODO define why constexpr
		/// </summary>
		/// <returns></returns>
		constexpr float Orientation_Radians() const
		{
			if (!_valid)
			{
				return DEGREES_MAX;// Degrees max is not a listed angle (same as 0 ofc). 
			}
			return HexAngles_Radians[_wedgeOrientation];
		}
	};

	struct HexWedgeGeometryArray
	{
	private:
		HexWedges::Geometry_HexWedge _wedgeArray[HEX_SIDE_COUNT];
	public:
		/// <summary>
		/// Produces an array of 6 invalid HexWedges. 
		/// </summary>
		HexWedgeGeometryArray()
		{
		
		}

		HexWedges::Geometry_HexWedge operator[] (UINT8 index) const
		{
			return _wedgeArray[index];
		}

		HexWedges::Geometry_HexWedge& operator[] (UINT8 index)
		{
			return _wedgeArray[index];
		}

		const HexWedges::Geometry_HexWedge GetWedgeAt(UINT8 index) const
		{
			if (index < HEX_SIDE_COUNT)
			{
				return _wedgeArray[index];
			}
			return HEX_WEDGE_GEOMETRY_INVALID;
		}
	};

	struct HexWedgeGeometry_PointyTop_Array
	{
	private:
		HexWedges::Geometry_HexWedge _wedgeArray[HEX_SIDE_COUNT]
		{
		
		};
	public:
		/// <summary>
		/// Produces an array of 6 invalid HexWedges. 
		/// </summary>
		HexWedgeGeometry_PointyTop_Array()
		{

		}

		HexWedges::Geometry_HexWedge operator[] (UINT8 index) const
		{
			return _wedgeArray[index];
		}

		HexWedges::Geometry_HexWedge& operator[] (UINT8 index)
		{
			return _wedgeArray[index];
		}

		const HexWedges::Geometry_HexWedge GetWedgeAt(UINT8 index) const
		{
			if (index < HEX_SIDE_COUNT)
			{
				return _wedgeArray[index];
			}
			return HEX_WEDGE_GEOMETRY_INVALID;
		}
	};

	struct HexWedgeGeometry_FlatTop_Array
	{
	private:
		HexWedges::Geometry_HexWedge _wedgeArray[HEX_SIDE_COUNT]
		{
	
		};
	public:
		/// <summary>
		/// Produces an array of 6 invalid HexWedges. 
		/// </summary>
		HexWedgeGeometry_FlatTop_Array()
		{

		}

		HexWedges::Geometry_HexWedge operator[] (UINT8 index) const
		{
			return _wedgeArray[index];
		}

		HexWedges::Geometry_HexWedge& operator[] (UINT8 index)
		{
			return _wedgeArray[index];
		}

		const HexWedges::Geometry_HexWedge GetWedgeAt(UINT8 index) const
		{
			if (index < HEX_SIDE_COUNT)
			{
				return _wedgeArray[index];
			}
			return HEX_WEDGE_GEOMETRY_INVALID;
		}
	};
#pragma endregion

#ifdef __cplusplus
}
#endif