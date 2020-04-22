#include "lock.h"

namespace base {

Lock::Lock(bool recursive)
{
  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr);  
  pthread_mutexattr_settype(&attr, recursive?PTHREAD_MUTEX_RECURSIVE_NP:PTHREAD_MUTEX_ERRORCHECK);
  pthread_mutex_init(&mutex_, &attr);
  pthread_mutexattr_destroy(&attr);
}

Lock::~Lock()
{
  pthread_mutex_destroy(&mutex_);
}

void Lock::Acquire()
{
  pthread_mutex_lock(&mutex_);
}

void Lock::Release()
{
  pthread_mutex_unlock(&mutex_);
}

pthread_mutex_t& Lock::get_mutex()
{ 
  return mutex_;
}

AutoLock::AutoLock(Lock& l)
  :l_(l)
{
  l_.Acquire();
}

AutoLock::~AutoLock()
{
  l_.Release();
}

}

