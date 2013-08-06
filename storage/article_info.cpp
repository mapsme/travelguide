#include "article_info.hpp"
#include "distance.hpp"

#include "../env/strings.hpp"
#include "../env/writer.hpp"
#include "../env/reader.hpp"

#include "../std/iterator.hpp"
#include "../std/algorithm.hpp"
#include "../std/cmath.hpp"


void ArticleInfo::GenerateKey()
{
  m_key = str::MakeNormalizeAndLowerUtf8(m_title);
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
  w.Write(m_parentUrl);

  WriteCoord(w, m_lat);
  WriteCoord(w, m_lon);
}

void ArticleInfo::Read(rd::Reader & r)
{
  r.Read(m_title);
  r.Read(m_url);
  r.Read(m_thumbnailUrl);
  r.Read(m_parentUrl);

  ReadCoord(r, m_lat);
  ReadCoord(r, m_lon);

  GenerateKey();
}

double ArticleInfo::Score(double currLat, double currLon) const
{
  if (m_lat != EMPTY_COORD && m_lon != EMPTY_COORD)
    return earth::Distance(m_lat, m_lon, currLat, currLon);
  else
    return 0.0;
}

void ArticleInfo::Swap(ArticleInfo & i)
{
  m_key.swap(i.m_key);
  m_url.swap(i.m_url);
  m_title.swap(i.m_title);
  m_thumbnailUrl.swap(i.m_thumbnailUrl);
  m_parentUrl.swap(i.m_parentUrl);
  std::swap(m_lat, i.m_lat);
  std::swap(m_lon, i.m_lon);
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
          m_url == r.m_url &&
          m_title == r.m_title &&
          m_thumbnailUrl == r.m_thumbnailUrl &&
          m_parentUrl == r.m_parentUrl &&
          EqualCoord(m_lat, r.m_lat) &&
          EqualCoord(m_lon, r.m_lon));
}
