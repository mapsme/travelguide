package com.example.travelguide.article;

import java.io.Serializable;

import com.example.travelguide.cpp.Storage;

@SuppressWarnings("serial")
public class ArticleInfo implements Serializable
{

  public ArticleInfo(String articleUrl, String iconUrl, String title,
                     String parent, double lat, double lon)
  {
    this.mArticleUrl = articleUrl;
    this.mIconUrl = iconUrl;
    this.mTitle = title;

    this.mParent = parent;
    this.mLat = lat;
    this.mLon = lon;
  }
  private final String mArticleUrl;
  private final String mIconUrl;
  private final String mTitle;

  private final String mParent;
  private final double mLat;
  private final double mLon;

  public String getName()      { return mTitle; }
  public String getIconUrl()   { return mIconUrl; }
  public String getArticleId() { return mArticleUrl; }
  public String getParent()    { return mParent; }

  public double getLat() { return mLat; }
  public double getLon() { return mLon; }

  public boolean isValidLatLon() { return Storage.isValidLatLon(mLat, mLon); }
}
