#pragma once

#include "storage.hpp"
#include "article_info.hpp"

#include "../std/map.hpp"


class ArticleInfoBuilder : public ArticleInfo
{
public:
  ArticleInfoBuilder(string const & title) : ArticleInfo(title) {}
  ArticleInfoBuilder(string const & title, ArticleInfoBuilder const & src, bool redirect)
    : ArticleInfo(title, src, redirect), m_parentUrl(src.m_parentUrl)
  {
  }

  string m_parentUrl;

  void Swap(ArticleInfoBuilder & b)
  {
    m_parentUrl.swap(b.m_parentUrl);
    ArticleInfo::Swap(b);
  }
};

inline void swap(ArticleInfoBuilder & b1, ArticleInfoBuilder & b2)
{
  b1.Swap(b2);
}


class StorageBuilder
{
  vector<ArticleInfoBuilder> m_info;
  map<string, size_t> m_url2info;

  void ProcessArticles();

public:
  void ParseEntries(string const & path);
  void ParseRedirects(string const & path);
  void ParseGeocodes(string const & path);

  void Add(ArticleInfoBuilder const & info);

  void Save(string const & path);

  void Assign(Storage & storage);

  bool operator == (Storage const & s) const;

  ArticleInfoBuilder * GetArticle(string const & url)
  {
    map<string, size_t>::const_iterator i = m_url2info.find(url);
    return (i == m_url2info.end() ? 0 : &m_info[i->second]);
  }
};

void InitStorageBuilderMock(StorageBuilder & builder);

class StorageMock : public Storage
{
public:
  StorageMock();
};
