#include "storage.hpp"

#include "../std/algorithm.hpp"
#include "../std/utility.hpp"


void Storage::QueryArticleInfos(vector<ArticleInfo> & out, string const & prefix,
                                double lat, double lon) const
{
  out.clear();

  typedef vector<ArticleInfo>::const_iterator IterT;
  pair<IterT, IterT> const range = equal_range(m_info.begin(), m_info.end(),
                                               prefix, ArticleInfo::LessPrefix());

  out.assign(range.first, range.second);
  sort(out.begin(), out.end(), ArticleInfo::LessScore(lat, lon));
}


StorageMock::StorageMock()
{
  ArticleInfo i1("London");
  i1.m_url = "London";
  i1.m_thumbnailUrl = "london.jpg";
  i1.m_parentUrl = "Europe -> Great Britain";
  m_info.push_back(i1);

  ArticleInfo i2("Lancaster");
  i2.m_url = "Lancaster";
  i2.m_thumbnailUrl = "lancaster.jpg";
  i2.m_parentUrl = "Europe -> Great Britain";
  m_info.push_back(i2);

  ArticleInfo i3("Great Britain");
  i3.m_url = "Great_Britain";
  i3.m_thumbnailUrl = "great_britain.jpg";
  i3.m_parentUrl = "Europe";
  m_info.push_back(i3);

  sort(m_info.begin(), m_info.end(), ArticleInfo::LessStorage());
}
