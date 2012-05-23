#ifndef SHREK_UTIL_H
#define SHREK_UTIL_H

#include <Common.h>
#include <sys/time.h>
void main_parser(const std::string filename, const std::string outfile);

namespace shrek {
//usec, sec = usec/1000000
int64_t currentTime();
char* getTimeStr(int64_t);
int getHour(int64_t date);

bool readWholeFile(std::string file, std::string& content);

bool genFileBasename(const std::string& url, std::string& fbase);

bool extractHref(std::string& fbase);

bool convUtf2Gb(std::string& fbase);

bool convGb2Utf(std::string& fbase);

bool parseTokens(std::string& fbase, int64_t& time);

bool extractJs(std::string& fbase, int64_t& time);

bool wget(std::string& url, std::string& fbase, int64_t& time);

bool svm(std::string& fbase, int64_t& time);

extern double gNetFlowKB;

extern const std::string dir;

}

#endif //SHREK_UTIL_H
