#pragma once

#include "storage.hpp"
#include "article_info.hpp"


class ArticleInfoBuilder : public ArticleInfo
{
public:
  ArticleInfoBuilder(string const & title) : ArticleInfo(title) {}

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

  void ProcessArticles();

public:
  void Add(ArticleInfoBuilder const & info);

  void Save(string const & path);

  void Assign(Storage & storage);

  bool operator == (Storage const & s) const;
};

void InitStorageBuilderMock(StorageBuilder & builder);

class StorageMock : public Storage
{
public:
  StorageMock();
};
