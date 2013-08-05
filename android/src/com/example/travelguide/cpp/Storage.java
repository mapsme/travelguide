package com.example.travelguide.cpp;

import com.example.travelguide.article.ArticleInfo;

public class Storage
{
  static
  {
    System.loadLibrary("tguide");
  }


  public native void query(String query, boolean useLocation, double lat, double lon);

  public native int getResultSize();

  public native ArticleInfo getArticleInfoByIndex(int index);
}
