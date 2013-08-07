#include "storage_builder.hpp"

#include "../env/writer.hpp"
#include "../env/assert.hpp"


void StorageBuilder::Add(ArticleInfoBuilder const & info)
{
  m_info.push_back(info);
}

void StorageBuilder::ProcessArticles()
{
  sort(m_info.begin(), m_info.end(), ArticleInfo::LessStorage());

  size_t const count = m_info.size();
  for (size_t i = 0; i < count; ++i)
  {
    size_t j = 0;
    for (; j < count; ++j)
    {
      if (i != j && m_info[i].m_parentUrl == m_info[j].m_url)
      {
        m_info[i].m_parentIndex = j;
        break;
      }
    }
  }
}

void StorageBuilder::Save(string const & path)
{
  ProcessArticles();

  wr::FileWriter w(path);
  size_t const count = m_info.size();
  w.Write(static_cast<uint32_t>(count));

  for (size_t i = 0; i < count; ++i)
    m_info[i].Write(w);
}

void StorageBuilder::Assign(Storage & storage)
{
  ProcessArticles();

  storage.m_info.assign(m_info.begin(), m_info.end());
}

bool StorageBuilder::operator == (Storage const & s) const
{
  if (m_info.size() != s.m_info.size())
    return false;

  for (size_t i = 0; i < m_info.size();++i)
    if (!(m_info[i] == s.m_info[i]))
      return false;

  return true;
}


void InitStorageBuilderMock(StorageBuilder & builder)
{
  ArticleInfoBuilder i1("London");
  i1.m_url = "London";
  i1.m_thumbnailUrl = "london.jpg";
  i1.m_parentUrl = "Great_Britain";
  i1.m_lat = 51.50726;
  i1.m_lon = -0.12765;
  builder.Add(i1);

  ArticleInfoBuilder i2("Lancaster");
  i2.m_url = "Lancaster";
  i2.m_thumbnailUrl = "lancaster.jpg";
  i2.m_parentUrl = "Great_Britain";
  i2.m_lat = 54.04839;
  i2.m_lon = -2.79904;
  builder.Add(i2);

  ArticleInfoBuilder i3("Great Britain");
  i3.m_url = "Great_Britain";
  i3.m_thumbnailUrl = "great_britain.jpg";
  i3.m_lat = 54.70235;
  i3.m_lon = -3.27656;
  builder.Add(i3);
}

StorageMock::StorageMock()
{
  StorageBuilder builder;
  InitStorageBuilderMock(builder);
  builder.Assign(*this);
}
