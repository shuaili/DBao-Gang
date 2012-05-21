#include <Thread.h>
using namespace std;
using namespace std::tr1;
struct WrapperArg {
    std::tr1::function<void ()> threadFunction;
};

void* Thread::wrapper(void* in) {
    WrapperArg* arg = (WrapperArg*)in;
    arg->threadFunction();
    return NULL;
}

Thread* Thread::createThread(const std::tr1::function<void ()>& threadFunction)
{
    Thread* thread = new Thread;
    WrapperArg* arg = new WrapperArg;
    arg->threadFunction = threadFunction;
    int rtn = pthread_create(&thread->_id, NULL,
                             &wrapper, arg);
    assert(rtn == 0);
    return thread;
}
