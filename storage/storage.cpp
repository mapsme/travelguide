#include "storage.hpp"

#include "../env/strings.hpp"
#include "../env/reader.hpp"
#include "../env/logging.hpp"

#include "../std/algorithm.hpp"
#include "../std/utility.hpp"
#include "../std/map.hpp"
#include "../std/iterator.hpp"


void Storage::Load(rd::Reader & reader)
{
  try
  {
    m_info.clear();

    uint32_t count;
    reader.Read(count);

    m_info.resize(count);
    for (uint32_t i = 0; i < count; ++i)
      m_info[i].Read(reader);
  }
  catch (file::FileException const & ex)
  {
    LOG(ERROR, (ex));

    // no way to continue
    throw;
  }
}

void Storage::Load(string const & path)
{
  rd::SequenceFileReader reader(path);
  Load(reader);
}

void Storage::QueryArticleInfo(string const & prefix, double lat, double lon)
{
  m_lastQuery.clear();

  if (prefix.empty())
  {
    size_t const count = m_info.size();
    m_lastQuery.reserve(count);

    // add all articles except redirects
    for (size_t i = 0; i < count; ++i)
      if (!m_info[i].m_redirect)
        m_lastQuery.push_back(i);
  }
  else
  {
    // find range of articles by input query
    typedef vector<ArticleInfo>::iterator IterT;
    pair<IterT, IterT> range = equal_range(m_info.begin(), m_info.end(),
                                           str::MakeNormalizeAndLowerUtf8(prefix),
                                           ArticleInfo::LessPrefix());

    // filter duplicating redirects
    map<string, size_t> theMap;
    typedef map<string, size_t>::iterator MapIterT;
    while (range.first != range.second)
    {
      size_t const ind = distance(m_info.begin(), range.first);
      pair<MapIterT, bool> res = theMap.insert(make_pair(range.first->m_url, ind));

      // replace redirect index with origin index
      if (!res.second && m_info[res.first->second].m_redirect && !m_info[ind].m_redirect)
        res.first->second = ind;

      ++range.first;
    }

    // assign results
    m_lastQuery.reserve(theMap.size());
    for (MapIterT i = theMap.begin(); i != theMap.end(); ++i)
      m_lastQuery.push_back(i->second);
  }

  // sort according to score
  sort(m_lastQuery.begin(), m_lastQuery.end(), LessScore(*this, lat, lon));
}

string Storage::FormatParentName(ArticleInfo const & info) const
{
  string res;

  ArticleInfo const * p = &info;
  while (p->m_parentIndex != ArticleInfo::NO_PARENT)
  {
    p = &m_info[p->m_parentIndex];
    if (!res.empty())
      res += " > ";
    res += p->m_title;
  }

  return res;
}

ArticleInfo const * Storage::GetTitleFromUrl(string const & url) const
{
  for (size_t i = 0; i < m_info.size(); ++i)
    if (!m_info[i].m_redirect && m_info[i].m_url == url)
      return &m_info[i];
  return 0;
}

bool Storage::LessScore::operator() (size_t i1, size_t i2) const
{
  double const s1 = Get(i1).Score(m_lat, m_lon);
  double const s2 = Get(i2).Score(m_lat, m_lon);

  if (s1 == s2)
    return m_storage.GetKey(i1) < m_storage.GetKey(i2);
  else
    return (s1 < s2);
}
