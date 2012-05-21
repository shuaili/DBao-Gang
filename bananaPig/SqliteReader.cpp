#include <SqliteReader.h>

using namespace std;
using namespace std::tr1;
namespace shrek {

SqliteReader::SqliteReader(string dbName) { 
    int rc = sqlite3_open(dbName.c_str(), &_db);
    if(rc) {
        cerr<<"Can not open database: "<<sqlite3_errmsg(_db)<<endl;
        sqlite3_close(_db);
        assert(false);
    }
}

SqliteReader::~SqliteReader() { 
    if(_db) {
        sqlite3_close(_db);
    }
}

int SqliteReader::placesCb(void *ptr, int argc, char **argv, char **azColName) {
    static int index = 0;
    SqliteReader* reader = (SqliteReader*)ptr;
    Url url;
    for ( int i = 0; i < argc; i++) {
        string col = string(azColName[i]);
        if(col == "id") {
            url._id = atoi(argv[i]);
        } else if(col == "url") {
            url._name = argv[i] ? argv[i] : "NULL";
        } else if(col == "visit_count") {
            url._visitCount = atoi(argv[i]);
        }
    }
    reader->_urlVec.push_back(url);
    reader->_placeid2index[url._id] = index;
    ++index;
    return 0;
}

void SqliteReader::readPlaces() {
    string query("select * from moz_places where visit_count>0 order by id");
    char* errMsg = NULL;
    int rc = sqlite3_exec(_db, query.c_str(), placesCb, this, &errMsg );
    if ( rc != SQLITE_OK ) {
        cerr<<"SQL error: "<<errMsg<<endl;
        sqlite3_free(errMsg);
    }
}

int SqliteReader::historiesCb(void *ptr, int argc, char **argv, char **azColName) {
    SqliteReader* reader = (SqliteReader*)ptr;
    int index;
    int id, pid;

    /* Process place_id first, so to find the right index to get the url from m_urlVector */
    assert(string(azColName[2]) == "place_id"); 
    pid = atoi(argv[2]);
    if(reader->_placeid2index.count(pid)) {
        index = reader->_placeid2index[pid];
    } else {
        return 0;
    }

    /* Process id */
    assert(string(azColName[0]) == "id");
    id = atoi(argv[0]);
	
    /* Process from_visit */
    assert(string(azColName[1]) == "from_visit");
    reader->_urlVec[index].insertFromVisit(atoi(argv[1]));
	
    /* Process visit_date */
    assert(string(azColName[3]) == "visit_date");
    string visitDate = argv[3];
    istringstream ss(visitDate);
    int64_t date;
    ss >> date;
    reader->_urlVec[index].insertVisitDate(date);
		
    /* Save the map from id to place_id in m_id2placeid */
    reader->_id2placeid[id] = pid;
    return 0;
}

void SqliteReader::readHistories() {
    string query("select * from moz_historyvisits order by place_id");
    char* errMsg = NULL;
    int rc = sqlite3_exec(_db, query.c_str(), historiesCb, this, &errMsg );
    if ( rc != SQLITE_OK ) {
        cerr<<"SQL error: "<<errMsg<<endl;
        sqlite3_free(errMsg);
    }
}

void SqliteReader::computeFromVisit() {
    _id2placeid[0] = 0;
    UrlVec::iterator iter = _urlVec.begin();
    for(; iter != _urlVec.end(); ++iter ) {
        set<int>& _fromVisit = iter->_fromVisit;
        set<int>::iterator i = _fromVisit.begin();
        for(; i != _fromVisit.end(); ++i ) {
            _fromVisit.insert(_id2placeid[*i]);
            int from = _id2placeid[*i];
            int index = _placeid2index[from];
            _urlVec[index]._frecency = _urlVec[index]._frecency + 1;
        }
    }
    //calculate out edges
}

void SqliteReader::operator() () {
    readPlaces();
    readHistories();
    computeFromVisit();
}

}

