#pragma once

#include "article_info.hpp"

#include "../std/vector.hpp"


class Storage
{
public:

  void Load(string const & path);

  void QueryArticleInfos(vector<ArticleInfo> & out, string const & prefix,
                         double lat = EMPTY_COORD, double lon = EMPTY_COORD) const;

  /// For unit tests only.
  ArticleInfo const & GetArticle(size_t i) const { return m_info[i]; }

  ArticleInfo const * GetParentArticle(ArticleInfo const & info) const
  {
    return (info.m_parentIndex != ArticleInfo::NO_PARENT ? &m_info[info.m_parentIndex] : 0);
  }

  string GetParentName(ArticleInfo const & info) const
  {
    ArticleInfo const * p = GetParentArticle(info);
    return (p ? p->m_title : string());
  }

protected:
  vector<ArticleInfo> m_info;

  friend class StorageBuilder;
};
