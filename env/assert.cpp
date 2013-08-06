#include "assert.hpp"

#include "../std/iostream.hpp"

#include <cassert>    // for assert
#include <stdlib.h>   // for abort


namespace dbg
{
  void FireAssert(SourceAddress const & sa, string const & msg)
  {
    cerr << sa.ToString() << msg << endl;
#ifdef DEBUG
    assert(false);
#else
    abort();
#endif
  }
}
