#include <ResultCache.h>
#include <Util.h>
#include <Record.h>
using namespace std;
using namespace std::tr1;
namespace shrek {

bool ResultCache::addUrl(string & url, int flag)
{
    string fbase;
    if(!genFileBasename(url, fbase)) { return false; }
    //using token to index
    string filename = fbase + ".token";
    string content;
    readWholeFile(filename, content);

    if(content.empty()) { return false; }
    cout<<"insertResult:"<<filename<<endl;

    ScopedLock lock(_mutex);
    _contentMap[content] = flag;
    Node node(currentTime(), flag);
    _urlMap[url] = node;
    return true;
}



bool ResultCache::needDetect(string & url)
{
//    if(url.find("dianping") == string::npos) { return false; }
    ScopedLock lock(_mutex);
    UrlMap::iterator iter = _urlMap.find(url);
    if(iter != _urlMap.end()) {
	return currentTime() - iter->second.vtime >= 300*1000000ll;
    }
    return true;
}




int ResultCache::search(string & url, int64_t& dtime, int64_t& jsTime, int64_t& tkTime) {
    if(sLatency != 0) {
        ScopedLock lock(_mutex);
        UrlMap::iterator iter = _urlMap.find(url);
        if(iter != _urlMap.end()) {
            if(currentTime() - iter->second.vtime < sLatency) {
                return iter->second.flag;
            }
        }
    }
    //first step : get html to file.htm
    string fbase;

    if(!genFileBasename(url, fbase)) {
        cout<<"ERROR:getFileBasename:"<<url<<endl;
        return FMiss;
    }
    string debugStr = "url:" + url + "\nfbase:" + fbase;
    if(!wget(url, fbase, dtime)) {
        cout<<"ERROR:wget:"<<debugStr<<endl;
        return FMiss;
    }

    // second step: to see whther it is in the table
    if(!extractJs(fbase, jsTime)) {
        cout<<"ERROR:extractJs:"<<debugStr<<endl;
        return FMiss;
    }
    
    if(!parseTokens(fbase, tkTime)) {
        cout<<"ERROR:parseTokens:"<<debugStr<<endl;
        return FMiss;
    }
	
    string content;
    readWholeFile(fbase + ".token", content);
    if(content.empty()) { return FGood; } //benign

    ScopedLock lock(_mutex);
    ContentMap::iterator iter = _contentMap.find(content);
    if(iter == _contentMap.end()) {
        //    parseTokens(fbase, tkTime);
        return FMiss;            // miss
    } 
    return iter->second;      //hit
}

}

