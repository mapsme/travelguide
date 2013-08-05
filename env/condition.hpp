#pragma once

#include "mutex.hpp"


namespace env
{

class Condition : private noncopyable
{
public:
  Condition();
  ~Condition();

  void Lock() { m_mutex.Lock(); }
  void TryLock() { m_mutex.TryLock(); }
  void Unlock() { m_mutex.Unlock(); }

  void Signal();
  void SignalAll();
  void Wait();

  class Guard
  {
    Condition & m_cond;
  public:
    Guard(Condition & c) : m_cond(c) { m_cond.Lock(); }
    ~Guard() { m_cond.Unlock(); }
  };

private:
  Mutex m_mutex;
  pthread_cond_t m_condition;
};

}
