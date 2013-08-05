#pragma once

#include <pthread.h>

#include "../std/noncopyable.hpp"


namespace env
{

class Condition;

class Mutex : private noncopyable
{
public:
  Mutex();
  ~Mutex();

  void Lock();
  bool TryLock();
  void Unlock();

  class Guard
  {
    Mutex & m_mutex;
  public:
    Guard(Mutex & m) : m_mutex(m) { m_mutex.Lock(); }
    ~Guard() { m_mutex.Unlock(); }
  };

private:
  pthread_mutex_t m_mutex;

  friend class Condition;
};

}
