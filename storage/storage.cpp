#include "storage.hpp"

#include "../env/strings.hpp"
#include "../env/reader.hpp"
#include "../env/logging.hpp"

#include "../std/algorithm.hpp"
#include "../std/utility.hpp"


void Storage::Load(string const & path)
{
  try
  {
    rd::SequenceFileReader reader(path);

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
