#include <Arbitrator.h>

using namespace std;
using namespace std::tr1;
namespace shrek {

void Arbitrator::operator()() {
    int64_t sleepTime = 0;
    int index = 1;

    RecordVec::iterator iter = _userQueue.begin();
    for(; iter != _userQueue.end(); ++iter) {
        cout<<"process "<<index<<"\t"<<iter->_name<<endl;
//        if(iter->_name.find("dianping") == string::npos) {continue;}
        Func func = bind(&Checker::checkRecord, &_checker, &(*iter));
        _pool->run(func, iter->_name);
        //set sleep time to get next url
        sleepTime = iter->_nextone;	//emma
        cerr<<"sleeptime: "<<sleepTime/1000000.0<<endl;
        assert(sleepTime >= 0);
	//	if(sleepTime > 600*1000000)
	//  sleepTime = 600*1000000;
        int64_t tb = currentTime();
	if(sleepTime < 5000000) {
            int64_t realSleepTime = min((int64_t)5000000, sleepTime);
            usleep(realSleepTime);
	} else {
	  usleep(5000000);    //sleep 5s
	  int64_t temp_sleep = 5000000ll;
	  while(temp_sleep < sleepTime) {
	    if(_detector.noTask())
	      break;
	    else
	      usleep(1000000); //sleep 1s
	    temp_sleep = temp_sleep + 1000000;
	  }
	}
        cout<<"accuracy sleeptime:"<<(currentTime() - tb)/1000000.0<<endl;
	//  sleep(1);
        ++index;				
    }
    cout<<"arbitrator ends!!!!!!!!!!!!"<<endl;
}

  void Arbitrator::warmUp(){
 
    Record record("http://www.kimiss.com/");
    _checker.checkRecord(&record);
    sleep(1);
    Record record1("http://www.kimiss.com/");
    _checker.checkRecord(&record1);
 
}

void Arbitrator::start(int num) {
    _pool = new ThreadPool(num);
    _pool->start();
}

void Arbitrator::stop() {
    _pool->stop();
}

ostream & operator<<(ostream & out, Arbitrator & a) {

  out<<"***********result detail**************"<<endl;

    RecordVec::iterator iter = a._userQueue.begin();
    int hit[H_COUNT] = {0};
    int64_t ptime = 0;
    int64_t pureptime = 0;
    int64_t dtime = 0;
    int64_t jstime = 0;
    int64_t svmtime = 0;
    int64_t ftime = 0;
    int64_t tktime = 0;
    double num = a._userQueue.size();
    string missUrls;
    for(; iter != a._userQueue.end(); ++iter) {
        out<<*iter<<endl;
        if(iter->_hit) { 
            ++hit[iter->_hit]; 
        } else {
            missUrls += iter->_name + "\n"; 
        }
        dtime += iter->_dtime;
        jstime += iter->_jsTime;
        svmtime += iter->_svmTime;
        tktime += iter->_tkTime;
	ftime += iter->_ftime;
        ptime += iter->_processTime;
        pureptime += iter->_pureProcessTime;
    }
    out<<"*****************miss****************"<<endl
       <<missUrls;

    out<<"****************summary**************"<<endl;
    out<<"total: "<<num<<endl;
    out<<"url hit: "<<hit[H_URL]<<endl;
    out<<"js hit: "<<hit[H_JS]<<endl;
    out<<"tk hit: "<<hit[H_TK]<<endl;
    out<<"null hit"<<hit[H_NULL]<<endl;
    out<<"hit rate: "<< (hit[H_URL] + hit[H_JS] + hit[H_TK])/num<<endl;
    out<<"total detected: "<<a._checker._detectUrlCount<<endl;
    out<<"deduped url:" <<a._detector.getDedup()<<endl;
    out<<"dtime: "<<dtime/num/1000.0<<endl;
    out<<"jstime: "<<jstime/num/1000.0<<endl;
    out<<"tktime: "<<tktime/num/1000.0<<endl;
    out<<"ftime: "<<ftime/num/1000.0<<endl;
    out<<"svmtime: "<<svmtime/num/1000.0<<endl;
    out<<"processtime: "<<ptime/num/1000.0<<endl;
    out<<"puretime: "<<pureptime/num/1000.0<<endl;

    return out;
}


}

