#pragma once

#include "article.hpp"

#include "../std/map.hpp"
#include "../std/string.hpp"

class ArticleStorage
{
public:
  virtual ~ArticleStorage() {}
  virtual bool GetArticleById(string const & id, Article & out) const = 0;
};

class ArticleStorageMock : public ArticleStorage
{
public:
  ArticleStorageMock(map<string, Article> articles) : m_articles(articles) {}
  virtual bool GetArticleById(string const & id, Article & out) const;
private:
  typedef map<string, Article> ArticleMap;
  ArticleMap m_articles;
};

class ArticleStorageTable : public ArticleStorage
{
public:
  virtual bool GetArticleById(string const & id, Article & out) const;
};

