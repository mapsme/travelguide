#pragma once

#include "../std/string.hpp"
#include "../std/algorithm.hpp"


namespace wr { class Writer; }
namespace rd { class Reader; }

const int EMPTY_COORD = 1000;


class ArticleInfo
{
  string m_key;
  void GenerateKey();

public:
  ArticleInfo()
    : m_length(0), m_parentIndex(NO_PARENT),
      m_lat(EMPTY_COORD), m_lon(EMPTY_COORD), m_redirect(false)
  {
  }

  ArticleInfo(string const & title)
    : m_title(title), m_length(0), m_parentIndex(NO_PARENT),
      m_lat(EMPTY_COORD), m_lon(EMPTY_COORD), m_redirect(false)
  {
    GenerateKey();
  }

  void SetAsRedirect(ArticleInfo const & src);

  static const int32_t NO_PARENT = -1;

  string m_title;
  string m_url;
  string m_thumbnailUrl;
  uint32_t m_length;
  int32_t m_parentIndex;  // NO_PARENT is the root article

  double m_lat, m_lon;

  bool m_redirect;

  void Write(wr::Writer & w) const;
  void Read(rd::Reader & r);

  /// Calculate score for info. Less is better.
  double Score(double currLat, double currLon) const;

  void Swap(ArticleInfo & i);

  bool operator == (ArticleInfo const & r) const;

  class LessScore
  {
    double m_lat, m_lon;
  public:
    LessScore(double lat, double lon) : m_lat(lat), m_lon(lon) {}
    bool operator() (ArticleInfo const & i1, ArticleInfo const & i2) const
    {
      return i1.Score(m_lat, m_lon) < i2.Score(m_lat, m_lon);
    }
  };

  struct LessStorage
  {
    bool operator() (ArticleInfo const & i1, ArticleInfo const & i2) const
    {
      return (i1.m_key < i2.m_key);
    }
  };

  class LessPrefix
  {
    int Compare(ArticleInfo const & info, string const & prefix) const
    {
      size_t const count = min(info.m_key.size(), prefix.size());
      for (size_t i = 0; i < count; ++i)
      {
        if (info.m_key[i] < prefix[i])
          return -1;
        else if (info.m_key[i] > prefix[i])
          return 1;
      }
      return (info.m_key.size() < prefix.size() ? -1 : 0);
    }

  public:
    bool operator() (ArticleInfo const & info, string const & prefix) const
    {
      return (Compare(info, prefix) == -1);
    }
    bool operator() (string const & prefix, ArticleInfo const & info) const
    {
      return (Compare(info, prefix) == 1);
    }
  };
};

inline void swap(ArticleInfo & a1, ArticleInfo & a2)
{
  a1.Swap(a2);
}

inline string ToString(ArticleInfo const & i)
{
  return i.m_title;
}
