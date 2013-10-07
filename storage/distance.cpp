#include "distance.hpp"

#include "../std/algorithm.hpp"
#include "../std/cmath.hpp"


namespace earth
{

double const PI = atan(1.0) * 4.0;

double Degree2Rad(double deg)
{
  return deg / 180.0 * PI;
}

double Rad2Degree(double rad)
{
  return rad * 180.0 / PI;
}

double GetDistanceOnSphere(double lat1Deg, double lon1Deg, double lat2Deg, double lon2Deg)
{
  double const lat1 = Degree2Rad(lat1Deg);
  double const lat2 = Degree2Rad(lat2Deg);
  double const lon1 = Degree2Rad(lon1Deg);
  double const lon2 = Degree2Rad(lon2Deg);

  double const dlat = sin((lat2 - lat1) / 2.0);
  double const dlon = sin((lon2 - lon1) / 2.0);
  double const y = dlat * dlat + dlon * dlon * cos(lat1) * cos(lat2);
  return 2.0 * atan2(sqrt(y), sqrt(max(0.0, 1.0 - y)));
}

void GetOffsetOnSphere(double lat1Deg, double lon1Deg, double azimuth, double angDist,
                       double & lat2Deg, double & lon2Deg)
{
  double const lat1 = Degree2Rad(lat1Deg);
  double const lon1 = Degree2Rad(lon1Deg);

  double const sinLat1 = sin(lat1);
  double const cosLat1 = cos(lat1);
  double const sinA = sin(angDist);
  double const cosA = cos(angDist);

  double const lat2 = asin(sinLat1*cosA + cosLat1*sinA*cos(azimuth));
  double const lon2 = lon1 + atan2(sin(azimuth)*sinA*cosLat1, cosA - sinLat1*sin(lat2));

  lat2Deg = Rad2Degree(lat2);
  lon2Deg = Rad2Degree(lon2);
}

}
