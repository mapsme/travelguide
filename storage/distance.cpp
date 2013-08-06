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

double DistanceOnSphere(double lat1Deg, double lon1Deg, double lat2Deg, double lon2Deg)
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

}
