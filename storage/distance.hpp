#pragma once

namespace earth
{

inline double RadiusM() { return 6378000; }

double Degree2Rad(double deg);

/// @name Distance
/// @param[in] lat1, lat2, lon1, lon2 - in degrees.
//@{
double DistanceOnSphere(double lat1Deg, double lon1Deg, double lat2Deg, double lon2Deg);

inline double Distance(double lat1Deg, double lon1Deg, double lat2Deg, double lon2Deg)
{
  return RadiusM() * DistanceOnSphere(lat1Deg, lon1Deg, lat2Deg, lon2Deg);
}
//@}

}
