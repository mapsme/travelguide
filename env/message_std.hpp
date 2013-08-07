#pragma once

#include "../std/vector.hpp"
#include "../std/sstream.hpp"


namespace std
{

inline string ToString(string const & s) { return s; }

template <class T> string ToString(vector<T> const & v)
{
  ostringstream ss;
  ss << "{ ";
  size_t const count = v.size();
  for (size_t i = 0; i < count; ++i)
  {
    ss << ToString(v[i]);
    if (i != count - 1)
      ss << ", ";
  }
  ss << " }";

  return ss.str();
}

}
