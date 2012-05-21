#ifndef __LOCK_H__
#define __LOCK_H__
#include <pthread.h>
#include <cassert>

class ThreadMutex
{
public:
    ThreadMutex(const pthread_mutexattr_t *mta = NULL) {
        pthread_mutex_init(&_mutex, mta);
    }
    
    ~ThreadMutex() {
        pthread_mutex_destroy(&_mutex);
    }
    
    int lock() {
        return pthread_mutex_lock(&_mutex);
    }

    int trylock () {
        return pthread_mutex_trylock(&_mutex);
    }

    int unlock() {
        return pthread_mutex_unlock(&_mutex);
    }
private:
    ThreadMutex(const ThreadMutex&);
    ThreadMutex& operator = (const ThreadMutex&);
protected:
    pthread_mutex_t _mutex;
};

class ThreadCond : public ThreadMutex
{
public:
    ThreadCond() {
        pthread_cond_init(&_cond, NULL);
    }

    ~ThreadCond() {
        pthread_cond_destroy(&_cond);
    }

    int wait()
    {
      return pthread_cond_wait(&_cond, &_mutex);
    }
    
    int signal() {
        return pthread_cond_signal(&_cond);
    }

    int broadcast() {
        return pthread_cond_broadcast(&_cond);
    }
private:
    pthread_cond_t  _cond;
};

class ScopedLock
{
private:
    ThreadMutex &_lock;
private:
    ScopedLock(const ScopedLock &);
    ScopedLock& operator= (const ScopedLock &);
    
public:
    explicit ScopedLock(ThreadMutex& lock) : _lock(lock) {
        int ret = _lock.lock();
        assert(ret == 0); (void) ret;
    }

    ~ScopedLock() {
        int ret = _lock.unlock();
        assert(ret == 0); (void) ret;
    }
};
#endif
