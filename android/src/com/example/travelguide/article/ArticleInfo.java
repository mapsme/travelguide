package com.example.travelguide.article;

public class ArticleInfo
{

  public ArticleInfo(String articleUrl, String iconUrl, String title)
  {
    this.mArticleUrl = articleUrl;
    this.mIconUrl = iconUrl;
    this.mTitle = title;
  }
  private String mArticleUrl;
  private String mIconUrl;
  private String mTitle;
  
  public String getName()      { return mTitle; }
  public String getIconUrl()   { return mIconUrl; }
  public String getArticleId() { return mArticleUrl; }
}
