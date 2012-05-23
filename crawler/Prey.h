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
    _htmlDiffCt = -1;
    _preHtmlKey = 0;
    _jsDiffCt = -1;
    _preJsKey = 0;
    _maxDtime = 0;
  }
    void growUp(size_t htmlKey, size_t jsKey, int64_t dtime) {
    ++_totalCt;
    if(htmlKey != _preHtmlKey) {
      ++_htmlDiffCt;
      _preHtmlKey = htmlKey;
    }
    if(jsKey != _preJsKey) {
        ++_jsDiffCt;
        _preJsKey = jsKey;
    }
    if(dtime > _maxDtime) {
      _maxDtime = dtime;
    }
  }

  std::string _url;
  int64_t _totalCt;
  int64_t _htmlDiffCt;
  int64_t _jsDiffCt;
  size_t _preHtmlKey;
  size_t _preJsKey;
  int64_t _maxDtime;
  //  ThreadMutex _mutex;
  friend std::ostream& operator<<(std::ostream& out, Prey& p) {
    out<<"*****************************"<<std::endl
       <<p._url<<std::endl
       <<"totalCt:"<<p._totalCt
       <<"\thtmlDiffCt:"<<p._htmlDiffCt
       <<"\tjsDiffCt:"<<p._jsDiffCt
       <<"\tmaxDtime:"<<p._maxDtime/1000000.0<<std::endl;
    return out;
  }
};

SHREK_TYPEDEF_PTR(Prey);

}

#endif //SHREK_PREY_H
