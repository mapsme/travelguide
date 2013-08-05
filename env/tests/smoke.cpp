#include <gtest/gtest.h>

#include "../file_handle.hpp"
#include "../file_system.hpp"
#include "../logging.hpp"

#include "../../std/algorithm.hpp"
#include "../../std/vector.hpp"


/// @note Do not edit formatting here (SRC() test):
//@{
namespace
{
  string GetSourceAddress()
  {
    return SRC().ToString();
  }
}

TEST(EnvSmoke, SourceAddress)
{
  string s = GetSourceAddress();
  size_t const beg = s.find_last_of('/');
  EXPECT_NE(beg, string::npos);
  s = s.substr(beg + 1);

  size_t const end = s.find_last_of(',');
  EXPECT_NE(end, string::npos);
  string const test = s.substr(0, end);
  EXPECT_EQ(test, "smoke.cpp, GetSourceAddress");

  ostringstream ss;
  ss << test << ", " << (__LINE__ - 17) << ": ";  // magic constant
  EXPECT_EQ(s, ss.str());
}
//@}


TEST(EnvSmoke, FileHandle)
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
