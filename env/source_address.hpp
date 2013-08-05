#pragma once

#include "../std/string.hpp"


namespace dbg
{

class SourceAddress
{
  char const * m_file;
  char const * m_func;
  int m_line;

public:
  SourceAddress(char const * file, char const * func, int line)
    : m_file(file), m_func(func), m_line(line)
  {
  }

  string ToString() const;
};

inline string ToString(SourceAddress const & sa)
{
  return sa.ToString();
}

}

#define SRC() ::dbg::SourceAddress(__FILE__, __FUNCTION__, __LINE__)
