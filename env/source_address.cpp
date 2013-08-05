#include "source_address.hpp"

#include "../std/sstream.hpp"


namespace dbg
{

string SourceAddress::ToString() const
{
  ostringstream ss;
  ss << m_file << ", " << m_func << ", " << m_line << ": ";
  return ss.str();
}

}
