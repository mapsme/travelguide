#pragma once

#include "../std/vector.hpp"
#include "../std/sstream.hpp"


namespace msg
{

template <class T> string ToString(vector<T> const & v)
{
  ostringstream ss;
  ss << "{ ";
  for (size_t i = 0; i < v.size(); ++i)
    ss << ToString(v[i]) << ", ";
  ss << " }";

  return ss.str();
}

}
