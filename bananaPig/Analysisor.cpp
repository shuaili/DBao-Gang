#include <Analysisor.h>
#include <Util.h>
#include <Detector.h>

using namespace std;
using namespace std::tr1;
namespace shrek {


Analysisor::Analysisor() {
    // open htmlcontent db for html analysis
    int rc = sqlite3_open("./external/htmlcontent.sqlite" , &_db);
    if( rc ) {
        cerr<<"Can not open database: "<<sqlite3_errmsg(_db)<<endl;
        sqlite3_close(_db);
        assert(false);
    }
}

Analysisor::~Analysisor() { 
    sqlite3_close(_db);
}

void Analysisor::operator() (Record* record) {
    sqlite3_stmt * stmt;
    const char *zTail;
    string url = record->_name;
    int64_t vtime = record->_vtime;

    string fbase;
    if(!genFileBasename(url, fbase)) { return; }

    if(url.find("weibo") != -1 && url.find("nojs")  == -1) {
        if(url.find("?") != -1) {
            url += "&nojs=1";
        } else {
            url += "?nojs=1";
        }
    }

    int upper = vtime/1000000 + 26000;
    int lower = vtime/1000000;
    

    sqlite3_prepare(_db, "SELECT html FROM content WHERE url like ?  and date > ? and date < ? ;", -1, &stmt, &zTail);
    sqlite3_bind_blob(stmt, 1, url.c_str(), url.size(), NULL);
    sqlite3_bind_int(stmt, 2, lower);
    sqlite3_bind_int(stmt, 3, upper);
    int ret = sqlite3_step(stmt);
    if(ret == SQLITE_ROW) {
        string filename = dir + fbase + ".db.html";
        ofstream fout(filename.c_str());
        const char* content = static_cast<const char*>(sqlite3_column_blob(stmt, 0));
        int len = sqlite3_column_bytes(stmt, 0);
        fout.write(content, len);
	fbase = fbase+".db";
    } else {
        cout<<"no html from the database, using the current page"<<endl;		
    }


    extractHref(fbase); //generate .an file which contains possible url that might jump to

    add2Detector(fbase);
}

void Analysisor::add2Detector(string& fbase) {
    string filename = dir + fbase + ".html.an";
    ifstream fin(filename.c_str());
    StrVec urls;
    string line;
    int pos;
    while(fin) {
        if(fin.eof()) break;
        getline(fin, line);
        if(line.empty()) { continue; }
        pos = line.rfind('#');
        if(pos != string::npos) {
            line = line.substr(0, pos);
        }
	StrVec::iterator result = find( urls.begin( ), urls.end( ), line);
	if(result == urls.end())
	  urls.push_back(line);
    }
    fin.close();
    if(urls.size() == 0) 
	return;
    cout<<"possible jump urls from"<<filename<<":   "<<endl;
    int c = min(urls.size() - 1, (size_t)16000);
    //    for(int i = c; i >= 0; i--) {
    for(int i = 0; i <= c; ++i) {
        (*_detector)(urls[i]);
	cout<<urls[i]<<endl;
    }
}

/*
void analyze_missedpage(string &url)
{
    int download = 0;
    if(url.find("weibo") != -1 && url.find("nojs") == -1)
    {
        if(url.find("?") != -1)
            url = url+"&nojs=1";
        else
            url = url+"?nojs=1";

        download = 1;
    }
	
    string fbase;
    if(!genFileBasename(url, fbase)) { return; }
    string filename, outputfilename;
    filename = filename_base + ".html";

    if(download)
    {
        string get_html("wget -N -q --timeout=10 --load-cookies=cookies.txt --user-agent=\"Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.9.2.3) Gecko/20100401 Firefox/3.6.3 (.NET CLR 3.5.30729)\" -O ");
        string url_quote = "\""+url+"\"";
        get_html = get_html + filename+" " + url_quote;
        system(get_html.c_str());	
    }


		
    if(filename.find("google") !=-1 || filename.find("weibo") !=-1)     //file convertion from utf-8 to gb2312
    {
        string str1("iconv -c -f utf-8 -t gb2312 ");
        string new_filename = filename+".conv";
        string to_execute = str1 + filename + " > " + new_filename;
        system(to_execute.c_str());
        filename = new_filename;
    }

		
    outputfilename = filename+".an";
    string excute = "htmlcxx "+filename;
    system(excute.c_str());     //generate .an file which contains possible url that might jump to

    ifstream fin(outputfilename.c_str());

    string line;

    string_vector temp_strVector;
    while(fin)
    {
        if(fin.eof()) break;
        getline(fin, line);
        if(line != "")
            temp_strVector.push_back(line);
    }
    fin.close();

    for(int i = temp_strVector.size()-1; i>=0; i--)
        pm_urlqueue->put_url(temp_strVector[i]);    //put relative url into url_queue. to lower the next request's miss rate

    //	pm_urlqueue->output();

}
*/
}
