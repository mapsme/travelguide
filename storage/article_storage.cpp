#include "article_storage.hpp"

bool ArticleStorageMock::GetArticleById(string const & id, Article & out) const
{
  ArticleMap::const_iterator it = m_articles.find(id);
  if (it == m_articles.end())
    return false;
  out = it->second;
  return true;
}
