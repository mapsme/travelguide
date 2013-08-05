#pragma once

#include "file_handle.hpp"


namespace wr
{

class Writer
{
public:
  virtual ~Writer() {}
  virtual void Write(void const * p, size_t size) = 0;
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
