#ifndef BASE_LOCK_H
#define BASE_LOCK_H

#include <pthread.h>

namespace base {

  class Lock
  {
  public:
    Lock(bool recursive = true);
    ~Lock();
    void Acquire();
    void Release();
    pthread_mutex_t& get_mutex();
  private:
    pthread_mutex_t mutex_;
  };

  class AutoLock
  {
  public:
    AutoLock(Lock& l);
    ~AutoLock();
  private:
    Lock& l_;
  };

}
#endif // !BASE_LOCK_H

