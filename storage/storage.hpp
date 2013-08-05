#pragma once

#include "article_info.hpp"

#include "../std/vector.hpp"


class Storage
{
public:
  void Load(string const & path);

  void QueryArticleInfos(vector<ArticleInfo> & out, string const & prefix,
                         double lat = INVALID_LAT, double lon = INVALID_LON) const;

protected:
  vector<ArticleInfo> m_info;
};


class StorageMock : public Storage
{
public:
  StorageMock();
};
