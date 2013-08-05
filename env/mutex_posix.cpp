#include "mutex.hpp"
#include "posix.hpp"


namespace env
{

Mutex::Mutex()
{
  CHECK_POSIX(::pthread_mutex_init(&m_mutex, 0));
}

Mutex::~Mutex()
{
  CHECK_POSIX(::pthread_mutex_destroy(&m_mutex));
}

void Mutex::Lock()
{
  CHECK_POSIX(::pthread_mutex_lock(&m_mutex));
}

bool Mutex::TryLock()
{
  return (0 == ::pthread_mutex_trylock(&m_mutex));
}

void Mutex::Unlock()
{
  CHECK_POSIX(::pthread_mutex_unlock(&m_mutex));
}

}
