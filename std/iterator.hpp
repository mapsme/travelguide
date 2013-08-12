#pragma once

#include <iterator>

using std::back_inserter;
using std::distance;

namespace impl
{
  template <class ContT>
  class BackInserterFn
  {
    ContT & m_cont;
  public:
    BackInserterFn(ContT & cont) : m_cont(cont) {}
    template <class T> void operator() (T const & t) { m_cont.push_back(t); }
  };
}

template <class ContT>
impl::BackInserterFn<ContT> MakeBackInserter(ContT & cont)
{
  return impl::BackInserterFn<ContT>(cont);
}
