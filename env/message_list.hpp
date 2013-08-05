#pragma once

#include "strings.hpp"


namespace msg
{

inline string ToString(char const * s) { return s; }
inline string ToString(string const & s) { return s; }

/// Override ToString function for your custom class in it's namespace.
/// Your function will be called according to the ADL lookup.
/// This is the default "last chance" implementation.
template <class T>
inline string ToString(T const & t)
{
  return str::ToString(t);
}

inline string MessageList()
{
  return string();
}

template <class T1>
inline string MessageList(T1 const & t1)
{
  return ToString(t1);
}

template <class T1, class T2>
inline string MessageList(T1 const & t1, T2 const & t2)
{
  return ToString(t1) + " " + ToString(t2);
}

template <class T1, class T2, class T3>
inline string MessageList(T1 const & t1, T2 const & t2, T3 const & t3)
{
  return MessageList(t1, t2) + " " + ToString(t3);
}

}
