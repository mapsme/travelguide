#include "storage_builder.hpp"

#include "../env/writer.hpp"
#include "../env/assert.hpp"
#include "../env/logging.hpp"

#include "../std/fstream.hpp"
#include "../std/iterator.hpp"


namespace
{

template <class ToDo>
void ProcessEntriesFile(string const & path, ToDo & toDo)
{
  ifstream fs(path.c_str());

  string str;
  vector<string> entries;

  while (!fs.eof())
  {
    getline(fs, str);
    if (str.empty())
      continue;

    entries.clear();
    str::Tokenize(str, "\t", back_inserter(entries));

    toDo(entries);
  }
}

string EncodeTitle(string const & s)
{
  string res(s);
  replace(res.begin(), res.end(), '_', ' ');
  return res;
}

class DoAddEntries
{
  StorageBuilder & m_storage;
public:
  DoAddEntries(StorageBuilder & storage) : m_storage(storage) {}

  void operator() (vector<string> const & entries)
  {
    CHECK(entries.size() == 8, (entries));

    ArticleInfoBuilder builder(EncodeTitle(entries[1]));
    builder.m_url = entries[0];
    builder.m_length = atoi(entries[2].c_str());
    CHECK(builder.m_length != 0, (entries[2]));
    builder.m_thumbnailUrl = entries[3];
    builder.m_parentUrl = entries[4];

    m_storage.Add(builder);
  }
};

class DoAddRedirects
{
  StorageBuilder & m_storage;
public:
  DoAddRedirects(StorageBuilder & storage) : m_storage(storage) {}

  void operator() (vector<string> const & entries)
  {
    CHECK(entries.size() == 4, (entries));

    ArticleInfoBuilder const * p = m_storage.GetArticle(entries[2]);
    if (p)
      m_storage.Add(ArticleInfoBuilder(EncodeTitle(entries[1]), *p, true));
    else
      LOG(WARNING, ("No article for url:", entries[2]));
  }
};

}

void StorageBuilder::ParseEntries(string const & path)
{
  DoAddEntries doAdd(*this);
  ProcessEntriesFile(path, doAdd);
}

void StorageBuilder::ParseRedirects(string const & path)
{
  DoAddRedirects doAdd(*this);
  ProcessEntriesFile(path, doAdd);
}

void StorageBuilder::Add(ArticleInfoBuilder const & info)
{
  m_info.push_back(info);
  if (!info.m_redirect)
    CHECK(m_url2info.insert(make_pair(info.m_url, m_info.size()-1)).second, (info.m_url));
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

  try
  {
    wr::FileWriter w(path);
    size_t const count = m_info.size();
    w.Write(static_cast<uint32_t>(count));

    for (size_t i = 0; i < count; ++i)
      m_info[i].Write(w);
  }
  catch (file::FileException const & ex)
  {
    LOG(ERROR, (ex));
  }
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
  i1.m_url = "London.html";
  i1.m_thumbnailUrl = "london.jpg";
  i1.m_parentUrl = "Great_Britain.html";
  i1.m_lat = 51.50726;
  i1.m_lon = -0.12765;
  builder.Add(i1);

  ArticleInfoBuilder i2("Lancaster");
  i2.m_url = "Lancaster.html";
  i2.m_thumbnailUrl = "lancaster.jpg";
  i2.m_parentUrl = "Great_Britain.html";
  i2.m_lat = 54.04839;
  i2.m_lon = -2.79904;
  builder.Add(i2);

  ArticleInfoBuilder i3("Great Britain");
  i3.m_url = "Great_Britain.html";
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