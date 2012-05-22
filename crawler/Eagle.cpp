#include <ThreadPool.h>
#include <Prey.h>
#include <Eagle.h>
#include <sys/time.h>
using namespace std;
using namespace std::tr1;
using namespace shrek;
hash<string> Eagle::sHash;

bool Eagle::readWholeFile(string& file, string& content) {
  ifstream fin(file.c_str());
  fin.seekg(0, ios::end);
  int len = fin.tellg();
  if(len <= -1) { 
    return false;
  }
  fin.seekg(0, ios::beg);
  content.resize(len);
  fin.read(const_cast<char*>(content.data()), len);
  fin.close();
  return true;
}

int64_t Eagle::currentTime() {
    struct timeval tval;
    gettimeofday(&tval, NULL);
    return (tval.tv_sec * 1000000LL + tval.tv_usec);
}

bool Eagle::wget(string& url, string& file) {
  string urlQuote = " \"" + url + "\" ";
  string cmd = "wget -q --tries=3 --timeout=5 --user-agent=\"Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.9.2.3) Gecko/20100401 Firefox/3.6.3 (.NET CLR 3.5.30729)\" " + urlQuote;
  cmd += " -O " + file;
  int ret = system(cmd.c_str());
  if(WTERMSIG(ret) == 6 || WEXITSTATUS(ret) != 0) {
    return false;
  }
  return true;
}

void Eagle::operator() (Prey* prey) {
  ostringstream os;
  os << pthread_self();
  string file = "internal/" + os.str();
  int64_t dtime = currentTime();
  if(!wget(prey->_url, file)) { 
    cout<<"ERROR:wget:"<<prey->_url<<endl;
    return; 
  }
  dtime = currentTime() - dtime;
  string content;
  if(!readWholeFile(file, content)) {
    cout<<"ERROR:readfile:"<<file<<endl;
    return;
  }
  if(content.empty()) {
    cout<<"ERROR:empty:"<<file<<endl;
    return;
  }
  size_t hashKey = sHash(content);
  prey->growUp(hashKey, dtime);
  return;
}
//thread safe?



