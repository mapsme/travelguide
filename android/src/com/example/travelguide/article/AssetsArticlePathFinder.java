package com.example.travelguide.article;

public class AssetsArticlePathFinder implements ArticlePathFinder
{

  @Override
  public String getPath(ArticleInfo info)
  {
    String pathInAssets = "file:///android_asset/" + info.getArticleId();
    return pathInAssets;
  }

}
