#ifndef SHREK_RESULTCACHE_H
#define SHREK_RESULTCACHE_H

#include <Common.h>

namespace shrek {

class ResultCache
{
public:
    ResultCache() {}
    ~ResultCache() {}
private:
    ResultCache(const ResultCache &);
    ResultCache& operator=(const ResultCache &);
public:
    const static int64_t sLatency = 0;
    typedef std::tr1::unordered_map<std::string, int> ContentMap;
    ContentMap _contentMap;
    struct Node
    {
        Node() {}
        Node(int64_t t, int f) { vtime = t; flag = f; }
        int64_t vtime;
	int flag;
    };
    typedef std::map<std::string, Node> UrlMap;
    UrlMap _urlMap;
    ThreadMutex _mutex;

    bool addUrl(std::string & url, int flag);
    int search(std::string& url) {
        int64_t tmp;
        return search(url, tmp, tmp, tmp);
    }
    int search(std::string & url, int64_t &dtime, int64_t& jsTime, int64_t& tkTime);

    bool needDetect(std::string & url);

};

SHREK_TYPEDEF_PTR(ResultCache);

}

#endif //SHREK_RESULTCACHE_H
