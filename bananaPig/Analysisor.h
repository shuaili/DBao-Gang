#ifndef SHREK_ANALYSISOR_H
#define SHREK_ANALYSISOR_H

#include <Common.h>
#include <sqlite3.h>
#include <Record.h>

namespace shrek {
class Detector;
class Analysisor
{
public:
    Analysisor();
    ~Analysisor();
private:
    Analysisor(const Analysisor &);
    Analysisor& operator=(const Analysisor &);
public:
    void setDetector(Detector* detector) { _detector = detector;}
    void add2Detector(std::string& fbase);
    void operator() (Record* record);
public:
    Detector* _detector;
    sqlite3* _db;
};

SHREK_TYPEDEF_PTR(Analysisor);

}

#endif //SHREK_ANALYSISOR_H
