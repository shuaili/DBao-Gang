#include <Record.h>

using namespace std;
using namespace std::tr1;
namespace shrek {

static const char* sHitTypeStr[] = {
    "H_MISS",
    "H_URL",
    "H_JS",
    "H_TK",
    "H_NULL"
};

ostream & operator<<(ostream & out, Record &a) {
        out<<"url name : "<<endl<<a._name<<endl;
        time_t date = time_t(a._vtime/1000000);
        tm * temp, *visitdate;
        temp = localtime(&date);
        visitdate = new tm;
        *visitdate = *temp;
        out<<"visit_time:     "<<a._vtime<<"\t"<<asctime(visitdate)<<endl;
        out<<"next request is in "<<a._nextone/1000000.0<<" seconds"<<endl;
	out<<"dtime:     "<<a._dtime<<endl;
	out<<"jstime:     "<<a._jsTime<<endl;
	out<<"tktime:     "<<a._tkTime<<endl;
	out<<"ftime:     "<<a._ftime<<endl;
	out<<"svmtime:     "<<a._svmTime<<endl;
        out<<"return time is :    "<<a._rtime<<endl;
        out<<"benign or malicious : "<<a._flag<<endl;

        out<<"hit type: "<< sHitTypeStr[a._hit]<<endl
           <<"process time:    "<<a._processTime<<endl
           <<"pure process time:    "<<a._pureProcessTime<<endl
           <<"*******************************************************"<<endl;
		
        return out;
    }

}

