#pragma once

#include <pthread.h>

#include "../std/noncopyable.hpp"


namespace env
{

class Thread : private noncopyable
{
public:
  class Runnable : private noncopyable
  {
    bool m_cancelled;

  public:
    Runnable() : m_cancelled(false) {}
    bool IsCancelled() const { return m_cancelled; }

    virtual void Run() = 0;
    virtual void Cancel() { m_cancelled = true; }
  };

  Thread() : m_runnable(0) {}

  void Create(Runnable * runnable);
  void Join();
  void Cancel();

private:
  Runnable * m_runnable;
  pthread_t m_handle;
};

}
