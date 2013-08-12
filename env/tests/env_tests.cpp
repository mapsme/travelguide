#include <gtest/gtest.h>

#include "../file_handle.hpp"
#include "../file_system.hpp"
#include "../logging.hpp"
#include "../strings.hpp"

#include "../../std/algorithm.hpp"
#include "../../std/vector.hpp"
#include "../../std/array.hpp"
#include "../../std/iterator.hpp"


/// @note Do not edit formatting here (SRC() test):
//@{
namespace
{
  string GetSourceAddress()
  {
    return SRC().ToString();
  }
}

TEST(Env, SourceAddress)
{
  string s = GetSourceAddress();
  size_t const beg = s.find_last_of('/');
  EXPECT_NE(beg, string::npos);
  s = s.substr(beg + 1);

  size_t const end = s.find_last_of(',');
  EXPECT_NE(end, string::npos);
  string const test = s.substr(0, end);
  EXPECT_EQ(test, "env_tests.cpp, GetSourceAddress");

  ostringstream ss;
  ss << test << ", " << (__LINE__ - 17) << ": ";  // magic constant
  EXPECT_EQ(s, ss.str());
}
//@}


TEST(Env, FileHandle)
{
  typedef file::FileHandle HandleT;

  char const * name = "file.bin";
  char const * buffer = "Some fellows believe in a dream until merry one!";

  try
  {
    {
      HandleT file(name, HandleT::WRITE_TRUNCATE);
      file.Write(buffer, strlen(buffer));
    }

    {
      HandleT file(name, HandleT::READ);

      size_t const n = strlen(buffer);
      vector<char> v(n);
      file.Read(v.data(), n);

      EXPECT_TRUE(equal(v.begin(), v.end(), buffer));
    }
  }
  catch (file::FileException const & ex)
  {
    LOG(ERROR, (ex));
  }

  EXPECT_TRUE(fs::DeleteFile(name));
}

TEST(Env, MakeNormalizeAndLowerUtf8)
{
  char const * arr[] = { "Atualização disponível", "Můžeš", "Über Karten", "Schließen" };
  char const * res[] = { "atualizacao disponivel", "muzes", "uber karten", "schliessen" };

  for (size_t i = 0; i < ArraySize(arr); ++i)
    EXPECT_EQ(str::MakeNormalizeAndLowerUtf8(arr[i]), res[i]);
}

TEST(Env, Tokenizer)
{
  string in = "\taaa, bbb?,\tccc ";
  string out[] = { "aaa", "bbb", "ccc" };

  vector<string> v;
  str::Tokenize(in, " ,\t?", MakeBackInserter(v));

  EXPECT_TRUE(equal(out, out + ArraySize(out), v.begin()));
}
