#pragma once

namespace earth
{

inline double RadiusM() { return 6378000; }
extern double PI;

/// @name Distance
/// @param[in] lat1, lat2, lon1, lon2 - in degrees.
//@{
double GetDistanceOnSphere(double lat1Deg, double lon1Deg, double lat2Deg, double lon2Deg);

inline double GetDistance(double lat1Deg, double lon1Deg, double lat2Deg, double lon2Deg)
{
  return RadiusM() * GetDistanceOnSphere(lat1Deg, lon1Deg, lat2Deg, lon2Deg);
}
//@}

/// @name Offset point.
/// @param[in] lat1, lat2, lon1, lon2 - in degrees.
/// @param[in] azimuth - azimuth in radians from north clockwise.
/// @param[in] angDist - angular distance on sphere in radians.
void GetOffsetOnSphere(double lat1Deg, double lon1Deg, double azimuth, double angDist,
                       double & lat2Deg, double & lon2Deg);

/// @param[in] distM - distance in meters.
inline void GetOffset(double lat1Deg, double lon1Deg, double azimuth, double distM,
                      double & lat2Deg, double & lon2Deg)
{
  GetOffsetOnSphere(lat1Deg, lon1Deg, azimuth, distM / RadiusM(), lat2Deg, lon2Deg);
}

}
