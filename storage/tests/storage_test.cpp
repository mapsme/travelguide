#include <gtest/gtest.h>

#include "storage_builder.hpp"
#include "distance.hpp"

#include "../env/message_std.hpp"
#include "../env/logging.hpp"
#include "../env/reader.hpp"
#include "../env/writer.hpp"
#include "../env/file_system.hpp"

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

TEST(Storage, PrefixQuery)
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

TEST(Storage, PrefixQuery_Utf8)
{
  StorageTest storage;

  char const * arrTitle[] = { "Über Karten", "Schließen" };
  size_t const count = ArraySize(arrTitle);

  storage.FillStorage(arrTitle, count);

  vector<ArticleInfo> out;
  storage.QueryArticleInfos(out, "ub");
  EXPECT_EQ(out.size(), 1);
  CheckBounds(out, "Über Karten", "Über Karten");

  storage.QueryArticleInfos(out, "schliessen");
  EXPECT_EQ(out.size(), 1);
  CheckBounds(out, "Schließen", "Schließen");
}

TEST(Storage, PrefixQuery_lowerCaseTest)
{
  StorageTest storage;

  char const * arrTitle[] = { "Great Britan"};
  size_t const count = ArraySize(arrTitle);

  storage.FillStorage(arrTitle, count);

  vector<ArticleInfo> out;
  storage.QueryArticleInfos(out, "");
  EXPECT_EQ(out.size(), 1);
  CheckBounds(out, "Great Britan", "Great Britan");

  storage.QueryArticleInfos(out, "g");
  EXPECT_EQ(out.size(), 1);
  CheckBounds(out, "Great Britan", "Great Britan");

  storage.QueryArticleInfos(out, "G");
  EXPECT_EQ(out.size(), 1);
  CheckBounds(out, "Great Britan", "Great Britan");

  storage.QueryArticleInfos(out, "gR");
  EXPECT_EQ(out.size(), 1);
  CheckBounds(out, "Great Britan", "Great Britan");

  storage.QueryArticleInfos(out, "GR");
  EXPECT_EQ(out.size(), 1);
  CheckBounds(out, "Great Britan", "Great Britan");
}

TEST(Storage, ArticleInfoRW)
{
  ArticleInfo i("Über Karten");
  i.m_url = "Éařízení";
  i.m_thumbnailUrl = "great_britain.jpg";
  i.m_lat = 5.67894;
  i.m_lon = 89.12345;

  char const * name = "article.dat";
  {
    wr::FileWriter w(name);
    i.Write(w);
  }
  {
    ArticleInfo test;
    rd::SequenceFileReader r(name);
    test.Read(r);

    EXPECT_EQ(i, test);
  }

  fs::DeleteFile(name);
}

TEST(Storage, StorageBuilderRW)
{
  StorageBuilder builder;
  InitStorageBuilderMock(builder);

  ArticleInfoBuilder i("Über Karten");
  i.m_url = "Éařízení";
  i.m_thumbnailUrl = "great_britain.jpg";
  i.m_parentUrl = "Schließen";
  i.m_lat = 5.67894;
  i.m_lon = 89.12345;
  builder.Add(i);

  char const * name = "storage.dat";
  builder.Save(name);

  Storage storage;
  storage.Load(name);

  EXPECT_EQ(builder, storage);

  ArticleInfo const * zero = 0;
  EXPECT_EQ(storage.GetParentArticle(storage.GetArticle(0)), zero);
  EXPECT_NE(storage.GetParentArticle(storage.GetArticle(1)), zero);
  EXPECT_NE(storage.GetParentArticle(storage.GetArticle(2)), zero);
  EXPECT_EQ(storage.GetParentArticle(storage.GetArticle(3)), zero);

  fs::DeleteFile(name);
}

TEST(Distance, Smoke)
{
  // Equator length from wiki.
  EXPECT_TRUE(fabs(earth::Distance(0, 0, 0, 180) / 1000.0 - 40075 / 2.0) < 1.0);
}
