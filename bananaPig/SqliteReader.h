#ifndef SHREK_SQLITEREADER_H
#define SHREK_SQLITEREADER_H

#include <Common.h>
#include <Url.h>
namespace shrek {
class SqliteReader
{
public:
    SqliteReader(std::string dbName);
    ~SqliteReader();
private:
    SqliteReader(const SqliteReader &);
    SqliteReader& operator=(const SqliteReader &);
public:
    sqlite3* _db;
    UrlVec _urlVec;	// all the urls
    IntMap _id2placeid;	// map historyvisits_id and place_id;
    IntMap _placeid2index;

    void operator() ();
    static int placesCb(void *NotUsed, int argc, char **argv, char **azColName);
    void readPlaces();
    static int historiesCb(void *NotUsed, int argc, char **argv, char **azColName);
    void readHistories();
    void computeFromVisit();
    friend std::ostream & operator<<(std::ostream & out, SqliteReader & a) {
//        std::copy(a._urlVec.begin(), a._urlVec.end(), std::ostream_iterator<Url>(out, "\n"));
        UrlVec::iterator iter = a._urlVec.begin();
        for(; iter != a._urlVec.end(); ++iter) {
            out<<(*iter)<<std::endl;
        }
        return out;
    }

};

SHREK_TYPEDEF_PTR(SqliteReader);

}

#endif //SHREK_SQLITEREADER_H
