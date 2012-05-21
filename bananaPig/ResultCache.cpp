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

    if(!content.empty()) { 
        cout<<"insertResultToken:"<<filename<<endl;
        ScopedLock lock(_mutex);
        _tokenMap[content] = flag;
    }

    filename = fbase + ".js";
    readWholeFile(filename, content);
    if(!content.empty()) { 
        cout<<"insertResultJs:"<<filename<<endl;
        ScopedLock lock(_mutex);
        _jsMap[content] = flag;
    }

    ScopedLock lock(_mutex);
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


int ResultCache::search(string & url, int64_t& dtime, int64_t& jsTime, int64_t& tkTime, HitType& hitType) {
    hitType = H_MISS;
    if(sLatency != 0) {
        ScopedLock lock(_mutex);
        UrlMap::iterator iter = _urlMap.find(url);
        if(iter != _urlMap.end()) {
            if(currentTime() - iter->second.vtime < sLatency) {
                hitType = H_URL;
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
    string content;
    readWholeFile(fbase + ".js", content);
    if(!content.empty()) {
        ScopedLock lock(_mutex);
        ContentMap::iterator iter = _jsMap.find(content);
        if(iter != _tokenMap.end()) {
            hitType = H_JS;
            return iter->second;
        }
    }
    
    if(!parseTokens(fbase, tkTime)) {
        cout<<"ERROR:parseTokens:"<<debugStr<<endl;
        return FMiss;
    }
	

    readWholeFile(fbase + ".token", content);
    if(content.empty()) { return FGood; } //benign

    ScopedLock lock(_mutex);
    ContentMap::iterator iter = _tokenMap.find(content);
    if(iter == _tokenMap.end()) {
        return FMiss;            // miss
    } 
    hitType = H_TK;
    return iter->second;      //hit
}

}

