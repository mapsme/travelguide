package com.example.travelguide.article;

public class AssetsArticlePathFinder implements ArticlePathFinder
{

  @Override
  public String getPath(ArticleInfo info)
  {
    final String pathInAssets = "file:///android_asset/" + info.getArticleId() + ".html";
    return pathInAssets;
  }

}
