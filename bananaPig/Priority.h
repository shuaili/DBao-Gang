#ifndef SHREK_PRIORITY_H
#define SHREK_PRIORITY_H

#include <Common.h>
#include <Record.h>
#include <Url.h>

namespace shrek {
typedef std::vector<Record> RecordVec;
class Priority
{
private:
    Priority(const Priority &);
    Priority& operator=(const Priority &);
public:
    const static int64_t sUsecPerDay = 60ll*60*24*1000000;
    const static int sTestNum = INT_MAX;
    const static int64_t sPriorityThreshold = 100;
    UrlVec& _urlVec;
    //testing data
    RecordVec _recordVec;
    UrlVec _detectUrls;
public:
    Priority(UrlVec& urlVec) : _urlVec(urlVec) { }
    ~Priority(){}

    void computeLocalPriority();	

    void setNextTime();

    void operator()();

    friend std::ostream & operator<<(std::ostream & out, Priority & a);
};

SHREK_TYPEDEF_PTR(Priority);

}

#endif //SHREK_PRIORITY_H
