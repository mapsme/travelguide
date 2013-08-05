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

}
