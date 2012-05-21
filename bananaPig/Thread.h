#ifndef __THREAD_H__
#define __THREAD_H__
#include <pthread.h>
#include <cassert>
#include <tr1/memory>
#include <tr1/functional>
#include <Common.h>
class Thread 
{
public:
    // when create thread fail, return null ThreadPtr
    // need hold return value else will hang to the created thread terminate
    static Thread* createThread(const std::tr1::function<void ()>& threadFunction);
private:
    static void* wrapper(void*);
public:
    pthread_t getId() const {return _id;}
    void join() {
        if (_id) {
            int ret = pthread_join(_id, NULL);
            (void) ret; assert(ret == 0);
        }
        _id = 0;
    }
public:
    ~Thread() {
        join();
    }
private:
    Thread() {_id = 0;} 
    pthread_t _id;
};
#endif
