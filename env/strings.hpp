#pragma once

#include "../std/string.hpp"
#include "../std/sstream.hpp"


namespace str
{

template <class T> string ToString(T const & t)
{
  ostringstream ss;
  ss << t;
  return ss.str();
}

string MakeNormalizeAndLowerUtf8(string const & s);

template <class IterT>
IterT Tokenize(string const & s, char const * delims, IterT out)
{
  size_t i = 0;
  while (i < s.size())
  {
    size_t j = s.find_first_of(delims, i);
    if (j == string::npos)
      j = s.size();
    if (j > i)
    {
      *out++ = s.substr(i, j-i);
      i = j+1;
    }
    else
      ++i;
  }

  return out;
}

}
