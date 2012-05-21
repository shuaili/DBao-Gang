#ifndef SHREK_COMMON_H_
#define SHREK_COMMON_H_

#include <cassert>
#include <iostream>
#include <string>
#include <stdint.h>
#include <tr1/memory>
#include <tr1/functional>
#include <map>
#include <tr1/unordered_map>
#include <limits.h>
#include <set>
#include <vector>
#include <list>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <sstream>
#include <Lock.h>
#include <Util.h>
#include <sqlite3.h>
typedef std::set<int64_t> TimeSet;
typedef std::map<int, int> IntMap;
typedef std::set<int> IntSet;
typedef std::vector<std::string> StrVec;
typedef std::set<std::string> StrSet;
#define SHREK_BEGIN_NAMESPACE(x) namespace shrek { namespace x{ 

#define SHREK_END_NAMESPACE(x) } }

#define SHREK_USE_ROOT_NAMESPACE using namespace shrek;

#define SHREK_USE_NAMESPACE(x) using namespace shrek::x

#define SHREK_NS(x) shrek::x

#define SHREK_ALIAS_NAMESAPCE(x, y) namespace shrek { namespace x = y; }

#define SHREK_TYPEDEF_PTR(x) typedef std::tr1::shared_ptr<x> x##Ptr

/* define for close assign operator and copy constructor;should not be called if not implemented */
#define COPY_CONSTRUCTOR(T) \
    T(const T &); \
    T & operator=(const T &)

#define DISALLOW_COPY_AND_ASSIGN(T)             \
    COPY_CONSTRUCTOR(T)

#define DELETE(x) do {             \
    if(x){                                  \
    delete x;                           \
    x = NULL;                           \
    }                                       \
    }while(0)

#define DELETE_ARRAY(x) do {             \
    if(x){                                  \
    delete [] x;                           \
    x = NULL;                           \
    }                                       \
    }while(0)

#endif /*SHREK_COMMON_H_*/
