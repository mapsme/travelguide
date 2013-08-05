#include "condition.hpp"
#include "posix.hpp"


namespace env
{

Condition::Condition()
{
  CHECK_POSIX(::pthread_cond_init(&m_condition, 0));
}

Condition::~Condition()
{
  CHECK_POSIX(::pthread_cond_destroy(&m_condition));
}

void Condition::Signal()
{
  CHECK_POSIX(::pthread_cond_signal(&m_condition));
}

void Condition::SignalAll()
{
  CHECK_POSIX(::pthread_cond_broadcast(&m_condition));
}

void Condition::Wait()
{
  CHECK_POSIX(::pthread_cond_wait(&m_condition, &m_mutex.m_mutex));
}

}
