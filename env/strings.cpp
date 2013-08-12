#include "strings.hpp"
#include "assert.hpp"

#include "../std/vector.hpp"

#include "../3rdparty/utf8proc/utf8proc.h"


namespace str
{

string MakeNormalizeAndLowerUtf8(string const & s)
{
  int const count = static_cast<int>(s.size());
  if (count == 0)
    return string();

  vector<int32_t> buffer(count);
  int sz = utf8proc_decompose(reinterpret_cast<uint8_t const *>(s.c_str()), count, buffer.data(), count,
                              UTF8PROC_CASEFOLD | UTF8PROC_DECOMPOSE | UTF8PROC_STRIPMARK);
  CHECK(sz >= 0 && sz <= count, ());

  sz = utf8proc_reencode(buffer.data(), sz, 0);
  return string(reinterpret_cast<char *>(buffer.data()), sz);
}

}
