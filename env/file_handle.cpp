#include "file_handle.hpp"
#include "logging.hpp"
#include "assert.hpp"
#include "posix.hpp"


namespace file
{

FileHandle::FileHandle(string const & name, Mode mode)
  : m_name(name), m_mode(mode)
{
  char const * const modes [] = {"rb", "wb", "r+b", "ab"};
  m_file = fopen(name.c_str(), modes[mode]);
  if (m_file)
    return;

  if (mode == WRITE_EXISTING)
  {
    // if file doesn't exist "r+b" fails
    m_file = fopen(name.c_str(), "wb");
    if (m_file)
      return;
  }

  THROWEX(FileException, (E2S()));
}

FileHandle::~FileHandle()
{
  if (m_file && fclose(m_file))
    LOG(WARNING, ("Error closing file", E2S()));
}

string FileHandle::E2S() const
{
  char const * s;
  switch (m_mode)
  {
  case READ: s = "Read"; break;
  case WRITE_TRUNCATE: s = "Write truncate"; break;
  case WRITE_EXISTING: s = "Write existing"; break;
  case APPEND: s = "Append"; break;
  }

  return m_name + "; " + s + "; " + env::GetCError();
}

static int64_t const INVALID_POS = -1;

uint64_t FileHandle::Size() const
{
  int64_t const pos = ftell64(m_file);
  if (pos == INVALID_POS)
    THROWEX(FileException, (E2S(), pos));

  if (fseek64(m_file, 0, SEEK_END))
    THROWEX(FileException, (E2S()));

  int64_t const size = ftell64(m_file);
  if (size == INVALID_POS)
    THROWEX(FileException, (E2S(), size));

  if (fseek64(m_file, pos, SEEK_SET))
    THROWEX(FileException, (E2S(), pos));

  ASSERT(size >= 0, ());
  return static_cast<uint64_t>(size);
}

void FileHandle::Read(void * p, size_t size)
{
  size_t const readed = fread(p, 1, size, m_file);
  if (readed != size || ferror(m_file))
    THROWEX(FileException, (E2S(), readed, size));
}

uint64_t FileHandle::Pos() const
{
  int64_t const pos = ftell64(m_file);
  if (pos == INVALID_POS)
    THROWEX(FileException, (E2S()));

  ASSERT(pos >= 0, ());
  return static_cast<uint64_t>(pos);
}

void FileHandle::Seek(uint64_t pos)
{
  if (fseek64(m_file, pos, SEEK_SET))
    THROWEX(FileException, (E2S(), pos));
}

void FileHandle::Write(void const * p, size_t size)
{
  size_t const written = fwrite(p, 1, size, m_file);
  if (written != size || ferror(m_file))
    THROWEX(FileException, (E2S(), written, size));
}

void FileHandle::Flush()
{
  if (fflush(m_file))
    THROWEX(FileException, (E2S()));
}

void FileHandle::Truncate(uint64_t size)
{
  int const res = ftruncate(fileno(m_file), size);
  if (res)
    THROWEX(FileException, (E2S(), size));
}

}
