#include <gtest/gtest.h>
#include "../std/utility.hpp"

#include "../article_storage.hpp"

TEST(ArticleStorageMock, ReturnsIfAnArticleExists)
{
  map<string, Article> articles;
  articles.insert(make_pair("SomeId", Article()));
  ArticleStorage * storage = new ArticleStorageMock(articles);
  Article article;
  EXPECT_TRUE(storage->GetArticleById("SomeId", article));
  EXPECT_FALSE(storage->GetArticleById("SomeIdThatDoesNotExist", article));
  delete storage;
}
