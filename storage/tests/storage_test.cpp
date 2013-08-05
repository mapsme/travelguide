#include <gtest/gtest.h>

#include "storage.hpp"

#include "../env/message_std.hpp"
#include "../env/logging.hpp"

#include "../std/array.hpp"


namespace
{

void CheckBounds(vector<ArticleInfo> const & v, string const & beg, string const & end)
{
  EXPECT_FALSE(v.empty());
  EXPECT_EQ(v.front().m_title, beg);
  EXPECT_EQ(v.back().m_title, end);
}

class StorageTest : public Storage
{
public:
  void FillStorage(char const * arr[], size_t size)
  {
    m_info.clear();

    for (size_t i = 0; i < size; ++i)
      m_info.push_back(ArticleInfo(arr[i]));

    sort(m_info.begin(), m_info.end(), ArticleInfo::LessStorage());
  }

  void CheckBounds(string const & beg, string const & end) const
  {
    ::CheckBounds(m_info, beg, end);
  }
};

}

TEST(Storage, PrefixQuery1)
{
  StorageTest storage;

  char const * arrTitle[] = { "London", "Lomdon", "Z", "a", "london", "las vegas", "los angeles" };
  size_t const count = ArraySize(arrTitle);

  storage.FillStorage(arrTitle, count);
  storage.CheckBounds("a", "Z");

  vector<ArticleInfo> out;
  storage.QueryArticleInfos(out, "");
  EXPECT_EQ(out.size(), count);
  storage.CheckBounds("a", "Z");

  storage.QueryArticleInfos(out, "l");
  EXPECT_EQ(out.size(), 5);
  CheckBounds(out, "las vegas", "los angeles");

  storage.QueryArticleInfos(out, "lo");
  EXPECT_EQ(out.size(), 4);
  CheckBounds(out, "Lomdon", "los angeles");

  storage.QueryArticleInfos(out, "lon");
  EXPECT_EQ(out.size(), 2);
  CheckBounds(out, "London", "london");

  storage.QueryArticleInfos(out, "lx");
  EXPECT_EQ(out.size(), 0);

  storage.QueryArticleInfos(out, "lor");
  EXPECT_EQ(out.size(), 0);
}

TEST(Storage, PrefixQuery2)
{
  StorageTest storage;

  char const * arrTitle[] = { "London", "Lancaster", "Great Britan" };
  size_t const count = ArraySize(arrTitle);

  storage.FillStorage(arrTitle, count);

  vector<ArticleInfo> out;
  storage.QueryArticleInfos(out, "l");
  EXPECT_EQ(out.size(), 2);
  CheckBounds(out, "Lancaster", "London");
}
