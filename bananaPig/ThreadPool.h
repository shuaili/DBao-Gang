#ifndef __THREAD_POOL_H__
#define  __THREAD_POOL_H__
#include <vector>
#include <deque>
#include <Lock.h>
#include <Thread.h>
#include <tr1/memory>
#include <tr1/functional>
typedef std::tr1::function<void()> Func;

struct Task  
{
    Task(){}
    Task(Func& f, std::string& u, int64_t t) {
        func = f; url = u; time = t;
    }
    Func func;
    std::string url;
    int64_t time;
};

class ThreadPool {
public:
    ThreadPool(int num) {
        _run = false;
        _threadNum = num;
        _dedup = 0;
    }
    void start();
    void stop();
    void run(Func f, std::string& url);
    bool noTask();
    int getDedup() { return _dedup; }
    int totalDetected();
private:
    void loop();
private:
    std::deque<Task> _tasks;
    std::vector<Thread*> _threads;
    std::tr1::unordered_map<std::string, int64_t> _urlTime;
    int _threadNum;
    int _dedup;
    ThreadCond _cond;
    bool _run;
};
#endif
