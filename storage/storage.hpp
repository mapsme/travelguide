#pragma once

#include "article_info.hpp"

#include "../std/vector.hpp"


class Storage
{
public:

  /// @name Used in generator.
  //@{
  void Add(ArticleInfo const & a);
  void Save(string const & path);
  //@}

  void Load(string const & path);

  void QueryArticleInfos(vector<ArticleInfo> & out, string const & prefix,
                         double lat = INVALID_LAT, double lon = INVALID_LON) const;

protected:
  void SortByKey();

  vector<ArticleInfo> m_info;
};


class StorageMock : public Storage
{
public:
  StorageMock();

  bool operator == (StorageMock const & r) const;
};
