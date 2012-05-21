#include <Priority.h>

using namespace std;
using namespace std::tr1;
namespace shrek {

void Priority::computeLocalPriority() {
    /* Firstly, find current time. */
    int64_t now  = 1336060691ll * 1000000; // usec
//    int64_t now = currentTime();
    cout<<"current time of test:      "<<now<<endl;
    int hour = getHour(now);
    /* Deadline is 15 days ago */
    int64_t deadline = now - 15*sUsecPerDay;
    int64_t testTime = now - 1*sUsecPerDay;         //emma
    cout<<"testing data starts after:     "<<getTimeStr(testTime)<<endl;

    int testNum = 0;
    /* Secondly, compute each urls priority. */
    UrlVec::iterator url = _urlVec.begin();
    //cout<<"number of urls from db: "<<_urlVec.size()<<endl;
    for(; url != _urlVec.end(); ++url) {
      bool detect = false;
        int priority = 0;
        if(url->_visitCount > 0) {
            TimeSet::iterator t = url->_visitDates.begin();

            for(; t != url->_visitDates.end(); ++t) {
                int64_t date = *t;
                if( date < deadline ) { continue; }
                if(date> testTime 
                   && testNum < sTestNum) { // generate testing data
                    Record record;
                    record._name = url->_name;
                    record._vtime = date;
                    _recordVec.push_back(record);
                    ++testNum;
                } else {

                    int weight = 0;
                    int points = 30 - (date - now)/sUsecPerDay;
                    int vhour = getHour(date);
                    int diff = hour - vhour;
                    if(diff > 0) {                  //weight could be 1, 2, 3
                        weight = diff/8 + 1;
                    } else {
                        weight = (24+diff)/8 + 1;
                    }
                    priority += weight * points;
		    if(priority > sPriorityThreshold) {
		      //detect = true;
		      url->_priority = priority;
		      _detectUrls.push_back(*url);
		    }
                }
            }
        }
	// url->_priority = priority;
	//if(detect) {
	//  _detectUrls.push_back(*url);
	//}
    }
}

void Priority::setNextTime() {
    if(_recordVec.empty()) { return; }
    sort(_recordVec.begin(), _recordVec.end(), RecordSort());
    int i = 0;
    for(; i < _recordVec.size() - 1; i++) {
        _recordVec[i]._nextone = _recordVec[i+1]._vtime - _recordVec[i]._vtime;
        if(_recordVec[i]._nextone < 0) {
            cerr<<"next time error: "<<i<<" || i+1: "<<_recordVec[i+1]._vtime
                <<" i: "<<_recordVec[i]._vtime<<endl;
        }
    }
}

void Priority::operator()() {
    computeLocalPriority();
    setNextTime();
    sort(_detectUrls.begin(), _detectUrls.end(), UrlSort());
}

std::ostream & operator<<(std::ostream & out, Priority & a) {
    UrlVec::iterator iter = a._urlVec.begin();
    out<<"urlVec"<<endl;
    for(; iter != a._urlVec.end(); ++iter) {
        out<<(*iter)<<endl;
    }
    
    if(!a._recordVec.empty()) {
        RecordVec::iterator iter = a._recordVec.begin();
	int i = 1;
	out<<"recordVec:"<<endl;
        for(; iter != a._recordVec.end(); ++iter) {
            out<<i<<endl<<(*iter)<<endl;
	    i++;
        }
    }
    return out;
}

}

