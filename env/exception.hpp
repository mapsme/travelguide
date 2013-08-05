#pragma once

#include "message_list.hpp"

namespace ex
{

class Exception
{
  string m_msg;
public:
  Exception(string const & msg) : m_msg(msg) {}
  string const & Msg() const { return m_msg; }
};

}

#define THROWEX(klass, message) throw klass(::msg::MessageList message)
