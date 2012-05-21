#ifndef SHREK_DETECTOR_H
#define SHREK_DETECTOR_H

#include <Common.h>
#include <Checker.h>
#include <ThreadPool.h>
#include <Url.h>
namespace shrek {

class Detector
{
public:
    Detector(Checker& checker)
        : _checker(checker) {}
    ~Detector() {}
public:
    Checker& _checker;
    ThreadPool* _pool;
    void warmUp();
    void start(int num);
    void stop();
    void operator()(std::string url);
    void operator()(UrlVec& url);
    bool noTask();
    int getDedup();
};

SHREK_TYPEDEF_PTR(Detector);

}

#endif //SHREK_DETECTOR_H
