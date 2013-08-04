package com.example.travelguide.article;

import java.util.List;

public interface ArticlesProvider
{
  List<ArticleInfo> queryByLocation();
  List<ArticleInfo> query(String query);
}
