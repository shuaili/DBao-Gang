#include <Util.h>

using namespace std;
using namespace std::tr1;
namespace shrek {

const string dir = "./internal/";
const string htmlcxx = "~/Programs/bin/htmlcxx ";
int getHour(int64_t date) {
    time_t d = date / 1000000;
    tm *ptr = localtime(&d);
    return ptr->tm_hour;
}

char* getTimeStr(int64_t date) {
    time_t d = date/1000000;
    return asctime(localtime(&d));
}

int64_t currentTime() {
    struct timeval tval;
    gettimeofday(&tval, NULL);
    return (tval.tv_sec * 1000000LL + tval.tv_usec);
}

bool  readWholeFile(std::string file, 
                   std::string& content) {
    file = dir + file;
    ifstream fin(file.c_str());
    fin.seekg(0, ios::end);
    int len = fin.tellg();
    if(len <= -1) { 
        cerr<<"read "<<file<<" fail"<<endl;
        return false;
    }
    fin.seekg(0, ios::beg);
    content.resize(len);
    fin.read(const_cast<char*>(content.data()), len);
    fin.close();
    return true;
}

bool genFileBasename(const std::string& url, std::string& fbase) {
    if(url.size() > 250) { return false; }
    for(int i = 0; i < url.size(); i++) {
        if((url[i] > '@' && url[i] < '[')
           || (url[i] > '`' && url[i] < '{')
           || (url[i] > '/' && url[i] < ':')) {
            fbase += url[i];
        }
    }
    return true;
}

bool convUtf2Gb(std::string& fbase) {
    string fin = dir + fbase + ".html";
    string fout = dir + fbase + ".conv.html";
    string cmd = "iconv -c -f utf-8 -t gb2312 ";
    cmd += fin + " > " + fout;
    string cmd2 = "remove " + fout + " " + fin;
    return system(cmd.c_str()) != -1
        && system(cmd2.c_str()) != -1;
}

bool convGb2Utf(std::string& fbase) {
    string fin = dir + fbase + ".html";
    string fout = dir + fbase + ".conv.html";
    string cmd = "iconv -c -f gb2312 -t utf-8 ";
    cmd += fin + " > " + fout;
    return system(cmd.c_str()) != -1;
}




bool extractHref(std::string& fbase) {
    string filename = dir + fbase + ".html";

    if(filename.find("baidu") != string::npos) {
      convGb2Utf(fbase);
      string convFile = dir + fbase + ".conv.html";
      string mv = "mv " + convFile + " " + filename;
      (void)system(mv.c_str());
    }
    string cmd = htmlcxx + filename;
    return system(cmd.c_str()) != -1;
}




bool parseTokens(std::string& fbase, int64_t& time) {
    static ThreadMutex mutex;
    time = currentTime();
    ScopedLock lock(mutex);
    string filename = dir + fbase + ".js";
    string fileout = dir + fbase + ".token";
    main_parser(filename, fileout);
    time = currentTime() - time;
    return true;
}

bool extractJs(std::string& fbase, int64_t& time) {
    time = currentTime();
    string filename = dir + fbase + ".html";
    string extractJsStr("external/extract.py ");
    extractJsStr += filename;
    int ret = system(extractJsStr.c_str());
    time = currentTime() - time;
    if(ret == 256) {
        return false;
    }
    return true;
}

bool wget(std::string& url, std::string& fbase, int64_t& time) {
    time = currentTime();
    string filename = dir + fbase + ".html";
    string wgetStr("wget -N -q --timeout=10 --load-cookies=cookies.txt --user-agent=\"Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.9.2.3) Gecko/20100401 Firefox/3.6.3 (.NET CLR 3.5.30729)\" -O ");
    string urlQuote = "\"" + url + "\"";
    wgetStr += filename + " " + urlQuote;
    int ret = system(wgetStr.c_str());
    time = currentTime() - time;
    if(WTERMSIG(ret) == 6 ) {
        return false;
    }
    return true;
}

bool svm(std::string& fbase, int64_t& time) {
    time = currentTime();
    string fin = dir + fbase + ".test";
    string fout = dir + fbase + ".predict";
    string svmStr = "external/svm-predict " + fin + " external/detection.model "+ fout;
    bool ret = system(svmStr.c_str()) != -1;
    time = currentTime() - time;
    return ret;
}

}

