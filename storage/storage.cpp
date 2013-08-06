#include "storage.hpp"

#include "../env/strings.hpp"
#include "../env/reader.hpp"
#include "../env/writer.hpp"
#include "../env/logging.hpp"

#include "../std/algorithm.hpp"
#include "../std/utility.hpp"


void Storage::Add(ArticleInfo const & a)
{
  m_info.push_back(a);
}

void Storage::Save(string const & path)
{
  SortByKey();

  wr::FileWriter w(path);
  size_t const count = m_info.size();
  w.Write(static_cast<uint32_t>(count));

  for (size_t i = 0; i < count; ++i)
    m_info[i].Write(w);
}

void Storage::Load(string const & path)
{
  rd::SequenceFileReader reader(path);

  m_info.clear();

  uint32_t count;
  reader.Read(count);

  m_info.resize(count);
  for (uint32_t i = 0; i < count; ++i)
    m_info[i].Read(reader);
}

void Storage::QueryArticleInfos(vector<ArticleInfo> & out, string const & prefix,
                                double lat, double lon) const
{
  out.clear();

  typedef vector<ArticleInfo>::const_iterator IterT;
  pair<IterT, IterT> const range = equal_range(m_info.begin(), m_info.end(),
                                               str::MakeNormalizeAndLowerUtf8(prefix), ArticleInfo::LessPrefix());

  out.assign(range.first, range.second);
  sort(out.begin(), out.end(), ArticleInfo::LessScore(lat, lon));
}

void Storage::SortByKey()
{
  sort(m_info.begin(), m_info.end(), ArticleInfo::LessStorage());
}


StorageMock::StorageMock()
{
  ArticleInfo i1("London");
  i1.m_url = "London";
  i1.m_thumbnailUrl = "london.jpg";
  i1.m_parentUrl = "Europe -> Great Britain";
  i1.m_lat = i1.m_lon = -66.666;
  m_info.push_back(i1);

  ArticleInfo i2("Lancaster");
  i2.m_url = "Lancaster";
  i2.m_thumbnailUrl = "lancaster.jpg";
  i2.m_parentUrl = "Europe -> Great Britain";
  i2.m_lat = i2.m_lon = 66.666;
  m_info.push_back(i2);

  ArticleInfo i3("Great Britain");
  i3.m_url = "Great_Britain";
  i3.m_thumbnailUrl = "great_britain.jpg";
  i3.m_parentUrl = "Europe";
  i3.m_lat = i3.m_lon = -66.666;
  m_info.push_back(i3);

  SortByKey();
}

bool StorageMock::operator == (StorageMock const & r) const
{
  if (r.m_info.size() != m_info.size())
    return false;
  for (size_t i = 0; i < m_info.size();++i)
    if (!(m_info[i] == r.m_info[i]))
      return false;
  return true;
}
