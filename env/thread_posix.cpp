#include "thread.hpp"
#include "posix.hpp"


namespace env
{

void * PThreadProc(void * p)
{
  Thread::Runnable * runnable = reinterpret_cast<Thread::Runnable *>(p);
  runnable->Run();

  ::pthread_exit(0);
  return 0;
}

void Thread::Create(Runnable * runnable)
{
  m_runnable = runnable;
  CHECK_POSIX(::pthread_create(&m_handle, 0, &PThreadProc, reinterpret_cast<void *>(runnable)));
}

void Thread::Join()
{
  CHECK_POSIX(::pthread_join(m_handle, 0));
}

void Thread::Cancel()
{
  if (m_runnable)
  {
    m_runnable->Cancel();
    Join();
  }
}

}
