#ifndef __THREAD_POOL_H__
#define  __THREAD_POOL_H__
#include <vector>
#include <deque>
#include <Lock.h>
#include <Thread.h>
#include <tr1/memory>
#include <tr1/functional>
//typedef std::tr1::function<void()> Func;

class ThreadPool {
public:
    ThreadPool(int num) {
        _run = false;
        _threadNum = num;
    }
    int size() { 
      ScopedLock lock(_cond); 
      return _tasks.size();
    }
    void start();
    void stop();
    void run(Func f);
private:
    void loop();
private:
    std::deque<Func> _tasks;
    std::vector<Thread*> _threads;
    int _threadNum;
    ThreadCond _cond;
    bool _run;
};
#endif
