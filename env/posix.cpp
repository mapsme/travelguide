#include "posix.hpp"
#include "logging.hpp"

#include "../std/cstring.hpp"

#include <cerrno>   // for errno


namespace env
{
  void CheckPosixResult(int res)
  {
    if (res != 0)
      LOG(WARNING, (strerror(res)));
  }

  char const * GetCError()
  {
    return strerror(errno);
  }
}
