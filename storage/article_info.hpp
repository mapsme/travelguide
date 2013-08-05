#pragma once

#include "storage_common.hpp"
#include <iostream>

struct ArticleInfo
{
  ArticleInfo() {}
  ArticleInfo(string const & url, string const & title, string const & thumbnailUrl, string const & parentPath)
      : m_url(url), m_title(title), m_thumbnailUrl(thumbnailUrl), m_parentPath(parentPath) {}

  string m_url;
  string m_title;
  string m_thumbnailUrl;
  string m_parentPath;
};

inline bool operator == (ArticleInfo const & a1, ArticleInfo const & a2)
{
  return a1.m_url == a2.m_url && a1.m_title == a2.m_title &&
          a1.m_thumbnailUrl == a2.m_thumbnailUrl && a1.m_parentPath == a2.m_parentPath;
}

// It's important that PrintTo() is defined in the SAME
// namespace that defines Bar.  C++'s look-up rules rely on that.
inline void PrintTo(ArticleInfo const & artInfo, ::std::ostream* os) {
   *os << "ArticleInfi {" << artInfo.m_url << ", "
     << artInfo.m_title << ", "
     << artInfo.m_thumbnailUrl << ", "
     << artInfo.m_parentPath   << "}";
}
