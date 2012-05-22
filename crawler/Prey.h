#ifndef SHREK_PREY_H
#define SHREK_PREY_H
#include <Common.h>
namespace shrek {

class Prey
{
public:
  Prey(std::string& url) {
    _url = url;
    //not count first preyUpon
    _totalCt = -1;
    _diffCt = -1;
    _preHashKey = 0;
    _maxDtime = 0;
  }
  void growUp(size_t hashKey, int64_t dtime) {
    ++_totalCt;
    if(hashKey != _preHashKey) {
      ++_diffCt;
      _preHashKey = hashKey;
    }
    if(dtime > _maxDtime) {
      _maxDtime = dtime;
    }
  }

  std::string _url;
  int64_t _totalCt;
  int64_t _diffCt;
  size_t _preHashKey;
  int64_t _maxDtime;
  //  ThreadMutex _mutex;
  friend std::ostream& operator<<(std::ostream& out, Prey& p) {
    out<<"*****************************"<<std::endl
       <<p._url<<std::endl
       <<"totalCt:"<<p._totalCt<<"\tdiffCt:"<<p._diffCt
       <<"\tmaxDtime:"<<p._maxDtime/1000000.0<<std::endl;
    return out;
  }
};

SHREK_TYPEDEF_PTR(Prey);

}

#endif //SHREK_PREY_H
