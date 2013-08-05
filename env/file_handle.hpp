#pragma once

#include "exception.hpp"

#include "../std/cstdio.hpp"
#include "../std/stdint.hpp"
#include "../std/noncopyable.hpp"


namespace file
{

struct FileException : public ex::Exception
{
  FileException(string const & msg) : ex::Exception(msg) {}
};

inline string ToString(FileException const & ex)
{
  return ex.Msg();
}

class FileHandle : private noncopyable
{
public:
  /// Do not change order (@see cpp FileHandle::FileHandle)
  enum Mode { READ = 0, WRITE_TRUNCATE, WRITE_EXISTING, APPEND };

  FileHandle(string const & fileName, Mode mode);
  ~FileHandle();

  uint64_t Size() const;
  uint64_t Pos() const;

  void Seek(uint64_t pos);

  void Read(void * p, size_t size);
  void Write(void const * p, size_t size);

  void Flush();
  void Truncate(uint64_t sz);

  string GetName() const { return m_name; }

private:
  FILE * m_file;
  string m_name;
  Mode m_mode;

  /// Convert last error to string.
  string E2S() const;
};

}
