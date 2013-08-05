#pragma once

#include "article_info.hpp"

#include "../std/vector.hpp"
#include "../std/string.hpp"

class IndexStorage
{
public:
  virtual ~IndexStorage() {}
  virtual void QueryArticleInfos(vector<ArticleInfoId> & out, string const & prefix,
                                 double lat = INVALID_LAT, double lon = INVALID_LON) const = 0;
};

class IndexStorageMock : public IndexStorage
{
public:
  virtual void QueryArticleInfos(vector<ArticleInfoId> & out, string const & prefix,
                                 double lat = INVALID_LAT, double lon = INVALID_LON) const;
};
