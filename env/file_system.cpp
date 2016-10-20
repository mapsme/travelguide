#include "file_system.hpp"
#include "../std/cstdio.hpp"

namespace fs
{

bool DeleteFile(string const & path)
{
  return (0 == remove(path.c_str()));
}

}
