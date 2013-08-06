#include <gtest/gtest.h>

#include "storage.hpp"

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

    SortByKey();
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
  i.m_url = "Great_Britain";
  i.m_thumbnailUrl = "great_britain.jpg";
  i.m_parentUrl = "Schließen";
  i.m_lat = 5.67894;
  i.m_lon = 89.12345;

  char const * name = "file.dat";
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

TEST(Storage, StorageReadWriteTest)
{
  StorageMock s1;

  ArticleInfo i("Über Karten");
  i.m_url = "Great_Britain";
  i.m_thumbnailUrl = "great_britain.jpg";
  i.m_parentUrl = "Schließen";
  i.m_lat = 5.67894;
  i.m_lon = 89.12345;
  s1.Add(i);

  char const * name = "storage_mock.dat";
  s1.Save(name);

  StorageMock s2;
  s2.Load(name);

  EXPECT_EQ(s1, s2);

  fs::DeleteFile(name);
}
