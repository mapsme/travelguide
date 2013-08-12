#include <gtest/gtest.h>

#include "storage_builder.hpp"
#include "distance.hpp"

#include "../env/logging.hpp"
#include "../env/reader.hpp"
#include "../env/writer.hpp"
#include "../env/file_system.hpp"

#include "../std/array.hpp"


TEST(ArticleInfo, PrefixMatch)
{
  ArticleInfo i("   'Loch Lomond' and \"The Trossachs\"    (National-Park)");

  EXPECT_FALSE(i.PrefixMatchExcept1stToken("loch"));
  EXPECT_TRUE(i.PrefixMatchExcept1stToken("lom"));
  EXPECT_FALSE(i.PrefixMatchExcept1stToken("and"));
  EXPECT_FALSE(i.PrefixMatchExcept1stToken("the"));
  EXPECT_TRUE(i.PrefixMatchExcept1stToken("trossachs"));
  EXPECT_TRUE(i.PrefixMatchExcept1stToken("national"));
  EXPECT_TRUE(i.PrefixMatchExcept1stToken("park"));
  EXPECT_FALSE(i.PrefixMatchExcept1stToken("parke"));

  EXPECT_TRUE(i.PrefixMatchExcept1stToken("the trossachs national park"));
}

namespace
{

class StorageTest : public Storage
{
public:
  void FillStorage(char const * arr[], size_t size)
  {
    m_info.clear();

    for (size_t i = 0; i < size; ++i)
      m_info.push_back(ArticleInfoBuilder(arr[i], arr[i], 0, 0));

    sort(m_info.begin(), m_info.end(), ArticleInfo::LessStorage());
  }

  void CheckBounds(string const & beg, string const & end) const
  {
    EXPECT_FALSE(m_info.empty());
    EXPECT_EQ(m_info.front().GetTitle(), beg);
    EXPECT_EQ(m_info.back().GetTitle(), end);
  }

  void CheckResultBounds(string const & beg, string const & end) const
  {
    EXPECT_FALSE(m_info.empty());
    EXPECT_FALSE(m_lastQuery.empty());

    EXPECT_EQ(GetResult(0).GetTitle(), beg);
    EXPECT_EQ(GetResult(GetResultsCount() - 1).GetTitle(), end);
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

  storage.QueryArticleInfo("");
  EXPECT_EQ(storage.GetResultsCount(), count);
  storage.CheckBounds("a", "Z");

  storage.QueryArticleInfo("l");
  EXPECT_EQ(storage.GetResultsCount(), 5);
  storage.CheckResultBounds("las vegas", "los angeles");

  storage.QueryArticleInfo("lo");
  EXPECT_EQ(storage.GetResultsCount(), 4);
  storage.CheckResultBounds("Lomdon", "los angeles");

  storage.QueryArticleInfo("lon");
  EXPECT_EQ(storage.GetResultsCount(), 2);
  storage.CheckResultBounds("London", "london");

  storage.QueryArticleInfo("lx");
  EXPECT_EQ(storage.GetResultsCount(), 0);

  storage.QueryArticleInfo("lor");
  EXPECT_EQ(storage.GetResultsCount(), 0);
}

TEST(Storage, PrefixQuery_Utf8)
{
  StorageTest storage;

  char const * arrTitle[] = { "Über Karten", "Schließen" };
  size_t const count = ArraySize(arrTitle);

  storage.FillStorage(arrTitle, count);

  storage.QueryArticleInfo("ub");
  EXPECT_EQ(storage.GetResultsCount(), 1);
  storage.CheckResultBounds("Über Karten", "Über Karten");

  storage.QueryArticleInfo("schliessen");
  EXPECT_EQ(storage.GetResultsCount(), 1);
  storage.CheckResultBounds("Schließen", "Schließen");
}

TEST(Storage, PrefixQuery_lowerCaseTest)
{
  StorageTest storage;

  char const * title = "Great Britain";
  storage.FillStorage(&title, 1);

  storage.QueryArticleInfo("");
  EXPECT_EQ(storage.GetResultsCount(), 1);
  storage.CheckResultBounds(title, title);

  storage.QueryArticleInfo("g");
  EXPECT_EQ(storage.GetResultsCount(), 1);
  storage.CheckResultBounds(title, title);

  storage.QueryArticleInfo("G");
  EXPECT_EQ(storage.GetResultsCount(), 1);
  storage.CheckResultBounds(title, title);

  storage.QueryArticleInfo("gR");
  EXPECT_EQ(storage.GetResultsCount(), 1);
  storage.CheckResultBounds(title, title);

  storage.QueryArticleInfo("GR");
  EXPECT_EQ(storage.GetResultsCount(), 1);
  storage.CheckResultBounds(title, title);
}

TEST(Storage, ArticleInfoRW)
{
  ArticleInfoBuilder builder("Über Karten", "Éařízení", 5.67894, 89.12345);
  ArticleInfo info(builder);

  char const * name = "article.dat";
  {
    wr::FileWriter w(name);
    info.Write(w);
  }
  {
    ArticleInfo test;
    rd::SequenceFileReader r(name);
    test.Read(r);

    EXPECT_EQ(info, test);
  }

  fs::DeleteFile(name);
}

TEST(Storage, StorageBuilderRW)
{
  StorageBuilder builder;
  builder.InitMock();

  ArticleInfoBuilder i("Über Karten", "Éařízení", 5.67894, 89.12345);
  i.m_parentUrl = "Schließen";
  builder.Add(i);

  char const * name = "storage.dat";
  builder.Save(name);

  Storage storage;
  storage.Load(name);

  EXPECT_EQ(builder, storage);

  ArticleInfo const * zero = 0;
  EXPECT_EQ(storage.GetParentForIndex(0), zero);
  EXPECT_NE(storage.GetParentForIndex(1), zero);
  EXPECT_EQ(storage.FormatParentName(storage.GetArticle(1)), "Great Britain");
  EXPECT_NE(storage.GetParentForIndex(2), zero);
  EXPECT_EQ(storage.FormatParentName(storage.GetArticle(2)), "Great Britain");
  EXPECT_EQ(storage.GetParentForIndex(3), zero);

  fs::DeleteFile(name);
}

TEST(Distance, Smoke)
{
  // Equator length from wiki.
  EXPECT_TRUE(fabs(earth::Distance(0, 0, 0, 180) / 1000.0 - 40075 / 2.0) < 1.0);
}
