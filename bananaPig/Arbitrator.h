#ifndef SHREK_ARBITRATOR_H
#define SHREK_ARBITRATOR_H

#include <Common.h>
#include <Checker.h>
#include <Record.h>
#include <Detector.h>
#include <ThreadPool.h>

namespace shrek {

class Arbitrator
{
public:
    Arbitrator(Checker& checker, RecordVec& userQueue, Detector& detector) 
        : _checker(checker)
        , _userQueue(userQueue)
        , _detector(detector) {}
    ~Arbitrator() {}
private:
    Arbitrator(const Arbitrator &);
    Arbitrator& operator=(const Arbitrator &);
public:
    Checker& _checker;
    RecordVec& _userQueue;
    Detector& _detector;
    ThreadPool* _pool;
    void warmUp();
    void operator()();
    void start(int num);
    void stop();

    friend std::ostream & operator<<(std::ostream & out, Arbitrator & a);

};

SHREK_TYPEDEF_PTR(Arbitrator);

}

#endif //SHREK_ARBITRATOR_H
