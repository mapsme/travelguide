#pragma once

#include "../std/string.hpp"
#include "../std/algorithm.hpp"


namespace wr { class Writer; }
namespace rd { class Reader; }

const int EMPTY_COORD = 1000;


class ArticleInfo
{
  void GenerateKey();
  static string Title2Key(string const & s);

protected:
  string m_key;
  string m_title;
  string m_url;
  uint32_t m_length;
  int32_t m_parentIndex;  // NO_PARENT is the root article
  bool m_redirect;
  double m_lat, m_lon;

  static const int32_t NO_PARENT = -1;

  /// Need access for protected fields in Build and Storage classes.
  friend class Storage;
  friend class StorageBuilder;

public:
  ArticleInfo()
    : m_length(0), m_parentIndex(NO_PARENT),
      m_redirect(false), m_lat(EMPTY_COORD), m_lon(EMPTY_COORD)
  {
  }

  ArticleInfo(string const & title)
    : m_title(title), m_length(0), m_parentIndex(NO_PARENT),
      m_redirect(false), m_lat(EMPTY_COORD), m_lon(EMPTY_COORD)
  {
    GenerateKey();
  }

  ArticleInfo(string const & title, ArticleInfo const & src, bool redirect)
    : m_title(title), m_url(src.m_url),
      m_length(src.m_length), m_parentIndex(NO_PARENT),
      m_redirect(redirect), m_lat(src.m_lat), m_lon(src.m_lon)
  {
    GenerateKey();
  }

  static string Prefix2Key(string const & s);

  string const & GetTitle() const { return m_title; }
  string GetUrl() const { return m_url + ".html"; }
  string GetThumbnailUrl() const { return m_url + ".jpg"; }

  bool IsRedirect() const { return m_redirect; }
  bool IsValidCoordinates() const;

  void Write(wr::Writer & w) const;
  void Read(rd::Reader & r);

  bool GetLatLon(double & lat, double & lon) const
  {
    if (IsValidCoordinates())
    {
      lat = m_lat;
      lon = m_lon;
      return true;
    }
    return false;
  }

  /// Calculate score for info. Less is better.
  double Score(double currLat, double currLon) const;

  void Swap(ArticleInfo & i);

  bool operator == (ArticleInfo const & r) const;

  /// @param[in] query should be simplified, lower case, utf8 string (matching by m_key).
  bool PrefixMatchExcept1stToken(string const & query) const;

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
  return i.GetTitle();
}
