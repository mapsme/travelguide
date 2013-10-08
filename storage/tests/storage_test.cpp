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

void CheckScore(ArticleInfoBuilder const & i1,
                ArticleInfoBuilder const & i2,
                double r)
{
  // generic score check for quite far objects
  double const dist = earth::GetDistance(i1.Lat(), i1.Lon(), i2.Lat(), i2.Lon());
  if (dist < 50.0)
    return;

  r = r * 1000.0;
  size_t const count = 4;
  double const deltaAzimuth = 2.0 * earth::PI / count;

  for (size_t i = 0; i < count; ++i)
  {
    // get estimate users position
    double lat, lon;
    earth::GetOffset(i1.Lat(), i1.Lon(), i * deltaAzimuth, r, lat, lon);

    // do check score if position closer to i1 more than 3 times.
    if (earth::GetDistance(lat, lon, i2.Lat(), i2.Lon()) >= 2.0 * r)
    {
      double const s1 = i1.Score(lat, lon);
      double const s2 = i2.Score(lat, lon);
      if (s1 > s2)
      {
        LOG(INFO, ("Scores:", s1, s2));
        LOG(INFO, ("Distance:", dist, r));
        LOG(INFO, (i1.Title(), ";", i2.Title()));

        EXPECT_TRUE(false);
      }
    }
  }
}

}

TEST(ArticleInfo, ScoreSmoke)
{
  typedef ArticleInfoBuilder AIB;
  AIB arr[] = {
    AIB("London", 51.5073219, -0.1276474, 251423),
    AIB("Glasgow", 55.86115, -4.24999, 152010),
    AIB("Manchester", 53.47910, -2.24457, 132021),
    AIB("Aberdeen", 57.14525, -2.09137, 131042),
    AIB("Leeds", 53.79737, -1.54376, 118763),
    AIB("Birmingham", 52.48136, -1.89807, 115464),
    AIB("Bristol", 51.45566, -2.59531, 107102),
    AIB("Belfast", 54.59694, -5.93016, 105649),
    AIB("Liverpool", 53.40547, -2.98052, 90401),
    AIB("Edinburgh", 55.94832, -3.19319, 74188),
    AIB("Brighton", 50.82217, -0.13767, 62403)
  };

  // Check that no side effects.
  for (size_t i = 0; i < ArraySize(arr); ++i)
  {
    EXPECT_EQ(arr[i].Score(arr[i].Lat(), arr[i].Lon()),
              arr[i].Score(arr[i].Lat(), arr[i].Lon()));
  }

  // Check for correct scoring around every city (with the set of approximate radius in km).
  double arrLen[] = { 0, 0.5, 1, 2, 5, 10, 15, 20, 30, 50, 100 };

  size_t const count = ArraySize(arr);
  for (size_t len = 0; len < ArraySize(arrLen); ++len)
    for (size_t i = 0; i < count; ++i)
      for (size_t j = 0; j < count; ++j)
      {
        if (i != j)
          CheckScore(arr[i], arr[j], arrLen[len]);
      }
}

/*
TEST(ArticleInfo, ScoreUK)
{
  StorageBuilder builder;
  builder.Load("../../guide_info/index.dat");

  double arrLen[] = { 0, 0.3, 0.5, 1, 2, 5, 10, 15, 20, 30 };

  size_t const count = builder.GetSize();
  for (size_t len = 0; len < ArraySize(arrLen); ++len)
    for (size_t i = 0; i < count; ++i)
      for (size_t j = 0; j < count; ++j)
      {
        if (i != j)
        {
          ArticleInfoBuilder const & i1 = builder.GetArticle(i);
          ArticleInfoBuilder const & i2 = builder.GetArticle(j);

          if (!i1.IsRedirect() && !i2.IsRedirect() &&
              i1.IsValidCoordinates() && i2.IsValidCoordinates())
          {
            CheckScore(i1, i2, arrLen[len]);
          }
        }
      }
}
*/

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
  EXPECT_TRUE(fabs(earth::GetDistance(0, 0, 0, 180) / 1000.0 - 40075 / 2.0) < 1.0);
}

TEST(Distance, OffsetPoint)
{
  double lat1 = 66.666;
  double lon1 = 66.666;
  double lat2, lon2;
  double dist = 66666.666;
  earth::GetOffset(lat1, lon1, 0.666, dist, lat2, lon2);

  EXPECT_TRUE(fabs(earth::GetDistance(lat1, lon1, lat2, lon2) - dist) < 1.0E-6);
}
