#ifndef SHREK_URL_H
#define SHREK_URL_H

#include <Common.h>

namespace shrek {

class Url
{
public:
    std::string _name;    // url
    int _id;	     // ID
    int _weight;	     // for url queue, the bigger the number is, the higher priority it will be
    TimeSet _visitDates; // Set, ordered
    std::set<int> _fromVisit;	// from what urls can jump to current url

    int _visitCount;
    int _priority;
    int _frecency;         //compute outs

    Url(const std::string& name = std::string()) {
        _name = name;
        _id = 0;
        _weight = 0;
        _frecency = 0;
        _visitCount = 0;
        _priority = 0;
    }
    ~Url(){}	// deconstructor
	

    Url & operator=(Url& rhs) {
        if ( this != &rhs ) {
	    _name = rhs._name;
            _id = rhs._id;
	    _visitCount = rhs._visitCount;
            _priority = rhs._priority;
            _visitDates = rhs._visitDates;
        }
        return *this;
    }

    void insertVisitDate(int64_t visitDate) {
        _visitDates.insert(visitDate);
    }

    void insertFromVisit(int id) {
        _fromVisit.insert(id);
    }

    friend std::ostream & operator<<(std::ostream & out, Url & a);

    friend bool operator == (Url &a, Url &b) {
        return a._priority == b._priority;
    }

    friend bool operator < (Url &a, Url &b) {
        return a._priority < b._priority;
    }

    friend bool operator > (Url &a, Url &b) {
        return a._priority > b._priority;
    }
};

struct UrlSort
{
    bool operator()(const Url& u1, const Url& u2) {
        return u1._priority < u2._priority;
    }
};
typedef std::vector<Url> UrlVec;

SHREK_TYPEDEF_PTR(Url);

}

#endif //SHREK_URL_H
