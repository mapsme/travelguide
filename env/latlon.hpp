#pragma once


namespace ll
{

inline bool ValidLat(double lat) { return -90.0 <= lat && lat <= 90.0; }
inline bool ValidLon(double lon) { return -180.0 <= lon && lon <= 180.0; }

}
