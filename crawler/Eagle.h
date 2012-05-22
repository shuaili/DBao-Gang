#ifndef SHREK_EAGLE_H
#define SHREK_EAGLE_H

#include <Common.h>

namespace shrek {
  struct Prey;
  struct Eagle
  {
  public:
    Eagle() {}
    ~Eagle() {}
  private:
    Eagle(const Eagle &);
    Eagle& operator=(const Eagle &);
  public:
    void operator() (Prey* prey);
  private:
    bool  readWholeFile(std::string& file, std::string& content);
    int64_t currentTime();
    bool wget(std::string& url, std::string& file);

    static std::tr1::hash<std::string> sHash;
  };


}

#endif //SHREK_EAGLE_H
