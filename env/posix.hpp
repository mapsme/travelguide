#pragma once


namespace env
{
  void CheckPosixResult(int res);
  char const * GetCError();
}

#define CHECK_POSIX(x) env::CheckPosixResult(x)
