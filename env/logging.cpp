#include "logging.hpp"

#include "../std/iostream.hpp"


namespace dbg
{

string ToString(LogPriority pr)
{
  static char const * arr[] = { "DEBUG", "INFO", "WARNING", "ERROR" };
  return arr[pr];
}

void Print(LogPriority pr, SourceAddress const & sa, string const & msg)
{
  cout << ToString(pr) << " " << sa.ToString() << msg << endl;
}

}
