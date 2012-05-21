#include <Checker.h>

using namespace std;
using namespace std::tr1;
namespace shrek {

void Checker::checkUrl(string url) {
//    if(url.find("dianping") == string::npos) { return;}
    if(!_result.needDetect(url))
        return;
    {
        ScopedLock lock(_cond);
        if(_checkingUrls.count(url)) {
            return;
        }
        _checkingUrls.insert(url);
    }
    cout<<"checkUrl:"<<url<<endl;
    ++_detectUrlCount;
    string fbase;
    if(!genFileBasename(url, fbase)) { return;}
  
    int flag = _result.search(url);
    if(flag == 0 ) { //miss
        // the page has already been wgeted, set the download to be false 
        int64_t tmp;
        flag = detect(url, tmp, tmp);
        if(flag != FFail) {
            _result.addUrl(url, flag);
        }
    }

    ScopedLock lock(_cond);
    StrSet::iterator it = _checkingUrls.find(url);
    _checkingUrls.erase(it);
    _cond.broadcast();
}

void Checker::checkRecord(Record* record) {
    string url = record->_name;
//    if(url.find("dianping") == string::npos) { return;}
    {
        ScopedLock lock(_cond);
        while(_checkingUrls.count(url)) {
            _cond.wait();
        }
        _checkingUrls.insert(url);
    }
    string fbase;
    if(!genFileBasename(url, fbase)) { return; }
    int64_t stime = currentTime();
    int64_t hitType;
    int flag = _result.search(url, record->_dtime, record->_jsTime, record->_tkTime, record->_hit);
    if(flag != FMiss ) {  // hit~~~ benign or malicious depends on flag.
    } else { //miss
        // the page has already been wgeted, set the download to be false 
        flag = detect(url, record->_svmTime, record->_ftime);
        if(flag != FFail) {
            _result.addUrl(url, flag);
        }
    }
    record->_flag = flag;
    int64_t now = currentTime();
    record->_rtime = now;
    record->_processTime = now - stime;
    record->_pureProcessTime = record->_processTime - record->_dtime - record->_jsTime;
    _analysisor(record);

    ScopedLock lock(_cond);
    StrSet::iterator it = _checkingUrls.find(url);
    _checkingUrls.erase(it);
    _cond.broadcast();
}

int Checker::detect(string & url, int64_t& svmTime, int64_t& ftime) {
    //first step : get html to file.html
    string fbase;
    if(!genFileBasename(url, fbase)) { return FFail; }
    // new to detect
    Predictor predictor(_qgrams);

    int ret = predictor.parseTokens(fbase);

    int flag;  // "1" good; "-1" malicious; "0" error
    if(ret) { // tokens less than 3
        flag = FGood;
    } else {
        predictor.computeFeature(ftime);
        flag = predictor.predict(svmTime);
    }
    return flag;
}

}

