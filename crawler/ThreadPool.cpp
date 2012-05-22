#include <Thread.h>
#include "ThreadPool.h"

using namespace std;
using namespace std::tr1;

void ThreadPool::start() {
    _run = true;
    int num = _threadNum;
    while(num--) {
      Func func = bind(&ThreadPool::loop, this);
      Thread* thread = Thread::createThread(func);
      assert(thread);
      _threads.push_back(thread);
    }

}

void ThreadPool::loop() {
    while(_run) {
      Func f;
        int64_t t;
        {
            ScopedLock lock(_cond);
            while(_run && _tasks.empty()) {
                _cond.wait();
            }
            if(!_run) { return; }
            f = _tasks.back();
            _tasks.pop_back();
        }
        if(f) { f(); }
    }
}

void ThreadPool::run(Func f) {
    if(!_run) return;
    if(_threads.empty()) { f(); }
    ScopedLock lock(_cond);
    _tasks.push_back(f);
    _cond.signal();
}

void ThreadPool::stop() {
    _run = false;
    _cond.broadcast();
    for(int i = 0; i < _threads.size(); ++i) {
        delete _threads[i];
    }
    
}
