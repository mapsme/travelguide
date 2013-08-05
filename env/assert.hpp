#pragma once

#include "message_list.hpp"
#include "source_address.hpp"


namespace dbg
{
  void FireAssert(SourceAddress const & sa, string const & msg);
}

#define CHECK(x, msg) do { if (x) {} else { ::dbg::FireAssert(SRC(), ::msg::MessageList msg) } } while (false)

#ifdef DEBUG
  #define ASSERT(x, msg) CHECK(x, msg)
#else
  #define ASSERT(x, msg)
#endif
