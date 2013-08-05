#pragma once

#include "../std/string.hpp"
#include "article_info.hpp"
#include "storage_common.hpp"

class ArticleInfoStorage
{
public:
  virtual ~ArticleInfoStorage() {}
  virtual bool GetArticleInfoById(ArticleInfo & out, ArticleInfoId const & id) = 0;
};

class ArticleInfoStorageMock: public ArticleInfoStorage
{
public:
  virtual bool GetArticleInfoById(ArticleInfo & out, ArticleInfoId const & id);
};
