#pragma once

#include "storage.hpp"
#include "article_info.hpp"

#include "../std/map.hpp"


class ArticleInfoBuilder : public ArticleInfo
{
public:
  explicit ArticleInfoBuilder(string const & title) : ArticleInfo(title)
  {
  }

  ArticleInfoBuilder(ArticleInfo const & info) : ArticleInfo(info)
  {
  }

  ArticleInfoBuilder(string const & title, ArticleInfoBuilder const & src, bool redirect)
    : ArticleInfo(title, src, redirect), m_parentUrl(src.m_parentUrl)
  {
  }

  void SetParams(vector<string> const & entries);
  void SetLatLon(double lat, double lon)
  {
    m_lat = lat;
    m_lon = lon;
  }

  /// @name Used for tests.
  //@{
  ArticleInfoBuilder(string const & title, string const & url, double lat, double lon)
    : ArticleInfo(title)
  {
    m_url = url;
    SetLatLon(lat, lon);
  }

  ArticleInfoBuilder(string const & title, double lat, double lon, uint32_t length)
    : ArticleInfo(title)
  {
    m_length = length;
    SetLatLon(lat, lon);
  }
  //@}

  string m_parentUrl;

  void Swap(ArticleInfoBuilder & b)
  {
    m_parentUrl.swap(b.m_parentUrl);
    ArticleInfo::Swap(b);
  }

  string const & Title() const { return m_title; }
  double Lat() const { return m_lat; }
  double Lon() const { return m_lon; }
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
  void Load(string const & path);

  void Assign(Storage & storage);

  bool operator == (Storage const & s) const;

  ArticleInfoBuilder * GetArticle(string const & url)
  {
    map<string, size_t>::const_iterator i = m_url2info.find(url);
    return (i == m_url2info.end() ? 0 : &m_info[i->second]);
  }

  size_t GetSize() const { return m_info.size(); }
  ArticleInfoBuilder const & GetArticle(size_t i) const { return m_info[i]; }

  /// For tests only.
  void InitMock();
};


class StorageMock : public Storage
{
public:
  StorageMock();
};
