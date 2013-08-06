#pragma once

#include "file_handle.hpp"


namespace rd
{

class Reader
{
public:
  virtual ~Reader() {}
  virtual void Read(void * p, size_t size) = 0;

  template <typename T> void Read(T & t)
  {
    Read(static_cast<void *>(&t), sizeof(T));
  }

  void Read(string & s)
  {
    uint32_t size;
    Read(size);

    s.resize(size);
    Read(&s[0], size);
  }
};


class SequenceFileReader : public Reader
{
  typedef file::FileHandle HandleT;
  HandleT m_file;

public:
  SequenceFileReader(string const & name)
    : m_file(name, HandleT::READ)
  {
  }

  using Reader::Read;
  virtual void Read(void * p, size_t size)
  {
    m_file.Read(p, size);
  }
};

}
