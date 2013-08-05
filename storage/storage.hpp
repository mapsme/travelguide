# pragma once

#include "article_info_storage.hpp"
#include "index_storage.hpp"

#include "../std/scoped_ptr.hpp"
#include "../std/vector.hpp"

class Storage
{
public:
  /// @note takes ownership of @link(ArticleInfoStorage) and @link(IndexStorage)
  Storage(ArticleInfoStorage * artInfoStorage, IndexStorage * indexStorage);

  void QueryArticleInfos(vector<ArticleInfo> & out, string const & prefix,
                         double lat = INVALID_LAT, double lon = INVALID_LON) const;

private:
  scoped_ptr<ArticleInfoStorage> m_articleInfoStorage;
  scoped_ptr<IndexStorage> m_indexStorage;
};
