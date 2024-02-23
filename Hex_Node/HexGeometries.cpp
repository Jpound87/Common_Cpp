///JLB
#include "pch.h"

#include "HexGeometries.h"

using namespace HexGeometries;

#ifdef __cplusplus    // If used by C++ code, 
extern "C"
{          // we need to export the C interface
#endif
	/// <summary>
	/// Data structure intended to store the general characteristics of the geometry of the hexes.
	/// </summary>
	struct iHexGeometry
	{

	private:
		HexCoordinates::HexOrientation _orientation;
		bool _valid;
		const UINT64 _step;
		const UINT32 _segmentLength;
		const double _marginVertical;
		const double _marginHorizontal;

		const HexWedges::Geometry_HexWedge _wedges[HexWedges::HEX_SIDE_COUNT];

	public:
		iHexGeometry(HexCoordinates::HexOrientation  orientation, UINT64 step) :
			_orientation(orientation),
			_segmentLength(step/2),
			_marginVertical(CalculateMargin_Vertical(step, _segmentLength, orientation)),
			_marginHorizontal(CalculateMargin_Horizontal(step, _segmentLength, orientation)),
			_wedges(HexWedges::LookupWedgeGeometries(orientation))
		{

		}

		/// <summary>
		/// The length of each edge and also the length from the center to any corner,
		/// since the Hexagon decomposes to 6 equalateral triangles. 
		/// </summary>
		const double SegmentLength() const
		{
			return _segmentLength;
		}

		/// <summary>
		/// The vertical distance between adjacent hexagon centers.
		/// </summary>
		/// <returns>The value of the vertical margin.</returns>
		const double VerticalMargin() const
		{
			return _marginVertical;
		}

		/// <summary>
		/// The horizontal distance between adjacent hexagon centers.
		/// </summary>
		/// <returns>The value oof the horizontal margin.</returns>
		const double HorizontalMargin() const
		{
			return _marginHorizontal;
		}
	};

	/// <summary>
	/// Data structure intended to store the general characteristics of the geometry of the hexes with double precision.
	/// </summary>
	struct dHexGeometry
	{

	private:
		HexCoordinates::HexOrientation _orientation;
		bool _valid;
		const double _step;
		const double _segmentLength;
		const double _marginVertical;
		const double _marginHorizontal;

		const HexWedges::Geometry_HexWedge _wedges[HexWedges::HEX_SIDE_COUNT];

	public:
		dHexGeometry(HexCoordinates::HexOrientation orientation, double step) :
			_orientation(orientation),
			_segmentLength(step / 2),
			_marginVertical(CalculateMargin_Vertical(step, _segmentLength, orientation)),
			_marginHorizontal(CalculateMargin_Horizontal(step, _segmentLength, orientation)),
			_wedges(HexCoordinates::HexOrientation.GenerateWedgeGeometries(orientation))
		{

		}

		/// <summary>
		/// The length of each edge and also the length from the center to any corner,
		/// since the Hexagon decomposes to 6 equalateral triangles. 
		/// </summary>
		const double SegmentLength() const
		{
			return _segmentLength;
		}

		/// <summary>
		/// The vertical distance between adjacent hexagon centers.
		/// </summary>
		/// <returns>The value of the vertical margin.</returns>
		const double VerticalMargin() const
		{
			return _marginVertical;
		}

		/// <summary>
		/// The horizontal distance between adjacent hexagon centers.
		/// </summary>
		/// <returns>The value oof the horizontal margin.</returns>
		const double HorizontalMargin() const
		{
			return _marginHorizontal;
		}
	};
#ifdef __cplusplus
}
#endif