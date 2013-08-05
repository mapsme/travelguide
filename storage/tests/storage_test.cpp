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
    ArticleInfo("London", "London", "stub_url.png"),
    ArticleInfo("Lancaster", "Lancaster", "stub_url.png"),
    ArticleInfo("Great_Britain", "Great Britain", "stub_url.png"),
  };

  EXPECT_EQ(vector<ArticleInfo>(&expected[0], &expected[0] + 3), artInfos);
}

TEST(Storage, PrefixQuery)
{
  Storage storage(new ArticleInfoStorageMock(), new IndexStorageMock());

  vector<ArticleInfo> artInfos;
  storage.QueryArticleInfos(artInfos, "Lo");

  EXPECT_EQ(artInfos.size(), 1);
  EXPECT_EQ(artInfos[0], ArticleInfo("London", "London", "stub_url.png"));
}
