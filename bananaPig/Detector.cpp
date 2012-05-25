#include <Detector.h>

using namespace std;
using namespace std::tr1;
namespace shrek {

void Detector::warmUp() {
    _checker.checkUrl("www.weibo.com");
    _checker.checkUrl("www.cosdna.com");
    _checker.checkUrl("www.taobao.com");
    _checker.checkUrl("http://www.baidu.com/");
    _checker.checkUrl("http://www.kimiss.com");
    _checker.checkUrl("http://www.dianping.com/");
}

int Detector::getDedup() {
    return _pool->getDedup();
}

void Detector::operator() (string url) {
    Func func = bind(&Checker::checkUrl, &_checker, url);
    _pool->run(func, url);
}

void Detector::operator() (UrlVec& urlVec) {
    for(int i = 0; i < urlVec.size(); ++i) {
      string url = urlVec[i]._name;
      Func func = bind(&Checker::checkUrl, &_checker, url);
      _pool->run(func, url);
    }
}

void Detector::stop() {
    _pool->stop();
}

void Detector::start(int num) {
    _pool = new ThreadPool(num);
    _pool->start();
}

bool Detector::noTask() {
  return _pool->noTask();
}


}

