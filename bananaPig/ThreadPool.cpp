#include "ThreadPool.h"
#include <Util.h>
using namespace std;
using namespace std::tr1;
using namespace shrek;
void ThreadPool::start() {
    _run = true;
    int num = _threadNum;
    while(num--) {
        Thread* thread = Thread::createThread(bind(&ThreadPool::loop, this));
        assert(thread);
        _threads.push_back(thread);
    }

}

void ThreadPool::loop() {
    while(_run) {
        Task task;
        int64_t t;
        {
            ScopedLock lock(_cond);
            while(_run && _tasks.empty()) {
                _cond.wait();
            }
            if(!_run) { return; }
            task = _tasks.back();
            _tasks.pop_back();
            if(_urlTime.find(task.url) == _urlTime.end()) {
                assert(false);
            }
            t = _urlTime[task.url];
            if(task.time < t) {
                ++_dedup;
                continue; }
        }
        if(task.func) { task.func(); }
    }
}

void ThreadPool::run(Func f, string& url) {
    if(!_run) return;
    if(_threads.empty()) { f(); }
    ScopedLock lock(_cond);
    int64_t t = currentTime();
    _urlTime[url] = t;
    _tasks.push_back(Task(f, url, t));
    _cond.signal();
}


bool ThreadPool::noTask()
{
  if(!_tasks.size())
    return true;
  else
    return false;
}

void ThreadPool::stop() {
    _run = false;
    _cond.broadcast();
    for(int i = 0; i < _threads.size(); ++i) {
        delete _threads[i];
    }
    
}
