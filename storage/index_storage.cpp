#include "index_storage.hpp"

void IndexStorageMock::QueryArticleInfos(vector<ArticleInfoId> & out,  string const & prefix,
                                         double /*lat*/, double /*lon*/) const
{
  out.clear();
  if (prefix.empty())
    for (uint32_t i = 0; i < 3; ++i)
      out.push_back(i);
  else if (prefix == "L")
    for (uint32_t i = 0; i < 2; ++i)
      out.push_back(i);
  else if (prefix.size() <= 6 && string("London").substr(prefix.size()) == prefix)
      out.push_back(1);
}
