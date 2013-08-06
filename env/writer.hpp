#pragma once

#include "file_handle.hpp"


namespace wr
{

class Writer
{
public:
  virtual ~Writer() {}
  virtual void Write(void const * p, size_t size) = 0;

  template <typename T> void Write(T const & t)
  {
    Write(static_cast<void const *>(&t), sizeof(T));
  }

  void Write(string const & s)
  {
    size_t const count = s.size();
    Write(static_cast<uint32_t>(count));
    Write(s.c_str(), count);
  }
};


class FileWriter : public Writer
{
  typedef file::FileHandle HandleT;
  HandleT m_file;

public:
  FileWriter(string const & name)
    : m_file(name, HandleT::WRITE_TRUNCATE)
  {
  }

  using Writer::Write;
  virtual void Write(void const * p, size_t size)
  {
    m_file.Write(p, size);
  }

  uint64_t Size()
  {
    m_file.Flush();
    return m_file.Size();
  }
};

}
