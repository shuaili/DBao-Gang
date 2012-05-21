#ifndef SHREK_CHECKER_H
#define SHREK_CHECKER_H

#include <Common.h>
#include <QGramsReader.h>
#include <ResultCache.h>
#include <Analysisor.h>
#include <Predictor.h>
#include <Record.h>
#include <Lock.h>

namespace shrek {
class Detector;
class Checker
{
public:
    Checker(QGramMap& qgrams) 
        : _qgrams(qgrams) {
        _detectUrlCount = 0;
    }
    ~Checker() {}
private:
    Checker(const Checker &);
    Checker& operator=(const Checker &);
    StrSet _checkingUrls;
    ThreadCond _cond;
public:
    int _detectUrlCount;
    ResultCache _result;
    Analysisor _analysisor;
    QGramMap& _qgrams;
    void setDetector(Detector* detector) { _analysisor.setDetector(detector); }
    void checkUrl(std::string url);
    void checkRecord(Record* record);
    int detect(std::string & url, int64_t& svmTime, int64_t& ftime);
};

SHREK_TYPEDEF_PTR(Checker);

}

#endif //SHREK_CHECKER_H
