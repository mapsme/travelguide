#include "article_info.hpp"

#include "../env/strings.hpp"

#include "../std/iterator.hpp"
#include "../std/algorithm.hpp"


void ArticleInfo::GenerateKey()
{
  m_key = str::MakeNormalizeAndLowerUtf8(m_title);
}

double ArticleInfo::Score(double currLat, double currLon) const
{
  /// @todo
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
