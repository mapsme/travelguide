#include "storage.hpp"
#include "../std/vector.hpp"


Storage::Storage(ArticleInfoStorage * artInfoStorage, IndexStorage * indexStorage)
{
  m_articleInfoStorage.reset(artInfoStorage);
  m_indexStorage.reset(indexStorage);
}

void Storage::QueryArticleInfos(vector<ArticleInfo> & out, string const & prefix,
                                double lat, double lon) const
{
  vector<ArticleInfoId> ids;
  m_indexStorage->QueryArticleInfos(ids, prefix, lat, lon);
  out.clear();
  out.resize(ids.size());
  for (size_t i = 0; i < ids.size(); ++i)
    m_articleInfoStorage->GetArticleInfoById(out[i], ids[i]);
}

