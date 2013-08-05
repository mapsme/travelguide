#include "article_info_storage.hpp"

bool ArticleInfoStorageMock::GetArticleInfoById(ArticleInfo & out, ArticleInfoId const & id)
{
  if (id > 2)
    return false;

  switch (id)
  {
  case 0:
    out.m_thumbnailUrl = "london.jpg";
    out.m_url = "London";
    out.m_title = "London";
    return true;
  case 1:
    out.m_thumbnailUrl = "lancaster.jpg";
    out.m_url = "Lancaster";
    out.m_title = "Lancaster";
    return true;
  case 2:
    out.m_thumbnailUrl = "great_britain.jpg";
    out.m_url = "Great_Britain";
    out.m_title = "Great Britain";
    return true;
  }
  return false;
}
