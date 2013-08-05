#pragma once

#include "message_list.hpp"
#include "source_address.hpp"


namespace dbg
{

enum LogPriority { LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERROR };
string ToString(LogPriority pr);

void Print(LogPriority pr, SourceAddress const & sa, string const & msg);

}

#define LOG(pr, message) ::dbg::Print(::dbg::LOG_##pr, SRC(), ::msg::MessageList message)
