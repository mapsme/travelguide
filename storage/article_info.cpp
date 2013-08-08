#include "article_info.hpp"
#include "distance.hpp"

#include "../env/strings.hpp"
#include "../env/writer.hpp"
#include "../env/reader.hpp"
#include "../env/latlon.hpp"
#include "../env/assert.hpp"

#include "../std/iterator.hpp"
#include "../std/algorithm.hpp"
#include "../std/cmath.hpp"
#include "../std/static_assert.hpp"


void ArticleInfo::GenerateKey()
{
  m_key = str::MakeNormalizeAndLowerUtf8(m_title);
}

bool ArticleInfo::IsValidCoordinates() const
{
  STATIC_ASSERT(EMPTY_COORD > 200);
  if (m_lat < 200.0 && m_lon < 200.0)
  {
    ASSERT(ll::ValidLat(m_lat) && ll::ValidLon(m_lon), ());
    return true;
  }
  return false;
}

namespace
{

void WriteCoord(wr::Writer & w, double d)
{
  w.Write(static_cast<int32_t>(d * 100000));
}

void ReadCoord(rd::Reader & r, double & d)
{
  int32_t i;
  r.Read(i);
  d = i / 100000.0;
}

}

void ArticleInfo::Write(wr::Writer & w) const
{
  w.Write(m_title);
  w.Write(m_url);
  w.Write(m_thumbnailUrl);
  w.Write(m_length);
  w.Write(m_parentIndex);

  WriteCoord(w, m_lat);
  WriteCoord(w, m_lon);

  w.Write(m_redirect);
}

void ArticleInfo::Read(rd::Reader & r)
{
  r.Read(m_title);
  r.Read(m_url);
  r.Read(m_thumbnailUrl);
  r.Read(m_length);
  r.Read(m_parentIndex);

  ReadCoord(r, m_lat);
  ReadCoord(r, m_lon);

  r.Read(m_redirect);

  GenerateKey();
}

namespace
{
double const DISTANCE_TRASHOLD = 2.0E5;         // 200 km
double const METERS_PER_LETTER_FACTOR = 10.0;
}

double ArticleInfo::Score(double currLat, double currLon) const
{
  double dist;
  double const inf = 2.0E7;   // half of equator

  if (IsValidCoordinates() && currLat != EMPTY_COORD && currLon != EMPTY_COORD)
  {
    dist = earth::Distance(m_lat, m_lon, currLat, currLon);
    if (dist > DISTANCE_TRASHOLD)
      dist = inf;
  }
  else
    dist = inf;

  return (dist - METERS_PER_LETTER_FACTOR * m_length);
}

void ArticleInfo::Swap(ArticleInfo & i)
{
  m_key.swap(i.m_key);
  m_title.swap(i.m_title);
  m_url.swap(i.m_url);
  m_thumbnailUrl.swap(i.m_thumbnailUrl);
  std::swap(m_length, i.m_length);
  std::swap(m_parentIndex, i.m_parentIndex);
  std::swap(m_lat, i.m_lat);
  std::swap(m_lon, i.m_lon);
  std::swap(m_redirect, i.m_redirect);
}

namespace
{
  bool EqualCoord(double d1, double d2)
  {
    return fabs(d1 - d2) < 1.0E-7;
  }
}

bool ArticleInfo::operator == (ArticleInfo const & r) const
{
  return (m_key == r.m_key &&
          m_title == r.m_title &&
          m_url == r.m_url &&
          m_thumbnailUrl == r.m_thumbnailUrl &&
          m_parentIndex == r.m_parentIndex &&
          EqualCoord(m_lat, r.m_lat) &&
          EqualCoord(m_lon, r.m_lon));
}
