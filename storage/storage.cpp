#include "storage.hpp"

#include "../env/strings.hpp"
#include "../env/reader.hpp"
#include "../env/logging.hpp"

#include "../std/algorithm.hpp"
#include "../std/utility.hpp"
#include "../std/iterator.hpp"
#include "../std/stdint.hpp"

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

void Storage::ResultsAccumulator::Add(size_t ind)
{
  pair<MapT::iterator, bool> res = m_map.insert(make_pair(m_storage.GetUrl(ind), ind));

  // replace redirect index with origin index
  if (!res.second && m_storage.IsRedirect(res.first->second) && !m_storage.IsRedirect(ind))
    res.first->second = ind;
}

bool Storage::ResultsAccumulator::IsExist(size_t ind) const
{
  return (m_map.find(m_storage.GetUrl(ind)) != m_map.end());
}

void Storage::ResultsAccumulator::GetResults(vector<size_t> & out, double lat, double lon) const
{
  size_t const count = out.size();
  out.reserve(count + m_map.size());

  for (MapT::const_iterator i = m_map.begin(); i != m_map.end(); ++i)
    out.push_back(i->second);

  vector<size_t>::iterator iStart = out.begin();
  advance(iStart, count);
  sort(iStart, out.end(), LessScore(m_storage, lat, lon));
}

void Storage::QueryArticleInfo(string const & prefix, double lat, double lon)
{
  m_lastQuery.clear();

  string query = ArticleInfo::Prefix2Key(prefix);
  if (query.empty())
  {
    size_t const count = m_info.size();
    m_lastQuery.reserve(count);

    // Add all articles except redirects
    for (size_t i = 0; i < count; ++i)
      if (!m_info[i].m_redirect)
        m_lastQuery.push_back(i);

    sort(m_lastQuery.begin(), m_lastQuery.end(), LessScore(*this, lat, lon));
  }
  else
  {
    // Find and add range of articles by matchin input query.
    typedef vector<ArticleInfo>::iterator IterT;
    pair<IterT, IterT> range = equal_range(m_info.begin(), m_info.end(), query,
                                           ArticleInfo::LessPrefix());

    ResultsAccumulator acc1(*this);
    while (range.first != range.second)
    {
      acc1.Add(distance(m_info.begin(), range.first));
      ++range.first;
    }
    acc1.GetResults(m_lastQuery, lat, lon);

    // Process all articles by matching 2nd, 3rd, ... tokens
    ResultsAccumulator acc2(*this);
    for (size_t i = 0; i < m_info.size(); ++i)
    {
      if (m_info[i].PrefixMatchExcept1stToken(query) && !acc1.IsExist(i))
        acc2.Add(i);
    }
    acc2.GetResults(m_lastQuery, lat, lon);
  }
}

string Storage::FormatParentName(ArticleInfo const & info, int maxDepth) const
{
  string res;

  ArticleInfo const * p = &info;
  int depth = 0;
  while (depth++ < maxDepth && p->m_parentIndex != ArticleInfo::NO_PARENT)
  {
    p = &m_info[p->m_parentIndex];
    if (!res.empty())
      res += " > ";
    res += p->m_title;
  }

  return res;
}

ArticleInfo const * Storage::GetArticleInfoFromUrl(string const & url) const
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
