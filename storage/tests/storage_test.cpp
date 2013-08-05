#include <gtest/gtest.h>

#include "storage.hpp"
#include "article_info_storage.hpp"
#include "index_storage.hpp"

TEST(Storage, Smoke)
{
  Storage storage(new ArticleInfoStorageMock(), new IndexStorageMock());

  vector<ArticleInfo> artInfos;
  storage.QueryArticleInfos(artInfos, "");

  ArticleInfo expected[] =
  {
    ArticleInfo("London", "London", "london.jpg"),
    ArticleInfo("Lancaster", "Lancaster", "lancaster.jpg"),
    ArticleInfo("Great_Britain", "Great Britain", "great_britain.jpg"),
  };

  EXPECT_EQ(vector<ArticleInfo>(&expected[0], &expected[0] + 3), artInfos);
}

TEST(Storage, PrefixQuery)
{
  Storage storage(new ArticleInfoStorageMock(), new IndexStorageMock());

  vector<ArticleInfo> artInfos;
  storage.QueryArticleInfos(artInfos, "Lo");

  EXPECT_EQ(artInfos.size(), 1);
  EXPECT_EQ(artInfos[0], ArticleInfo("London", "London", "london.jpg"));
}
