#include <Url.h>

using namespace std;
using namespace std::tr1;
namespace shrek {

ostream& operator<<(ostream & out, Url & a) {
    out<<a._id<<" : "<<a._name<<endl;

    out<<"visit_count:  "<<a._visitCount<<endl;

    out<<"visit_date : "<<endl;
    TimeSet::iterator iter = a._visitDates.begin();
    for(; iter != a._visitDates.end(); ++iter ) {
        time_t date = *iter;
        tm * temp, *visitdate;
        temp = localtime(&date);
        visitdate = new tm;
        *visitdate = *temp;
        out<<asctime(visitdate)<<"               "<<date<<endl;
    }

    out<<"from_visit:"<<endl;
    std::set<int>::iterator it2 = a._fromVisit.begin();
    for(; it2 != a._fromVisit.end(); ++it2 ) {
        out<<(*it2)<<endl;
    }
        
    out<<"priority:"<<a._priority<<endl;
    out<<"to :"<<a._frecency<<endl;
    out<<endl<<endl;
    return out;
}


}

