#ifndef SHREK_URLRECORD_H
#define SHREK_URLRECORD_H

#include <Common.h>

namespace shrek {
enum { FGood = 1, FMiss = 0,  FMali = -1, FFail = -2, };

class Record
{
public:
    std::string _name;
    int64_t _vtime;
    int64_t _nextone;
    int64_t _rtime;
    int _flag; // 1 good, -1 malicious;
    bool _hit;  // hit or miss
    int64_t _dtime;
    int64_t _jsTime;
    int64_t _svmTime;
    int64_t _tkTime;
    int64_t _ftime;
    int64_t _processTime;
    int64_t _pureProcessTime;
public:
    Record(const std::string& name = std::string()) {
        _name = name;
        _vtime = 0;
        _nextone = 0;
        _rtime = 0;
        _dtime = 0;
        _jsTime = 0;
        _svmTime = 0;
        _tkTime = 0;
	_ftime = 0;
        _flag = 0;
        _hit = false;
        _processTime = 0;
        _pureProcessTime = 0;
    }
    ~Record(){}
    Record & operator = (Record &rhs ) {
        if(this != &rhs) {
            _name = rhs._name;
            _vtime = rhs._vtime;
            _nextone = rhs._nextone;
            _rtime = rhs._rtime;
        }
        return *this; 
    }

    friend bool operator == (Record &a, Record &b) {
        return a._vtime== b._vtime;
    }

    friend bool operator != (Record &a, Record &b) {
        return a._vtime != b._vtime;
    }

    friend bool operator < (const Record &a, const Record &b) {
        return a._vtime < b._vtime;
    }

    friend bool operator > (Record &a, Record &b) {
        return a._vtime > b._vtime;
    }

    friend std::ostream & operator<<(std::ostream & out, Record &a) ;
};
typedef std::vector<Record> RecordVec;

struct RecordSort 
{
    bool operator()(const Record& r1, const Record& r2) {
        return r1._vtime < r2._vtime;
    }
};
SHREK_TYPEDEF_PTR(Record);

}

#endif //SHREK_URLRECORD_H
