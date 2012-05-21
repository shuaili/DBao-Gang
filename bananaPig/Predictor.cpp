#include <Predictor.h>
#include <Record.h>
using namespace std;
using namespace std::tr1;
namespace shrek {
  const string Predictor::sDir = "internal/";
int Predictor::parseTokens(string& fbase) {
    _qgramSet.clear();
    _fbase = fbase;
    string filename = sDir + fbase + ".token";
    ifstream fin(filename.c_str());
    string line;
    vector<string> tokens;
    int i = 0;
    while(fin) {
        if(fin.eof()) break;
        getline(fin, line);
        while(i < line.size()) {
            int start = i;
            while (!isspace(line[i]) && i < line.size()) { i++; }
            int len = i - start;
            if(len) {
                string str = line.substr(start, len);
                tokens.push_back(str);
            } else { //fix bug: there are whitespace at the begining of the file
                break;
            }
            while (isspace(line[i])) { i++; }
        }
    }
    fin.close();
	
    // if token number is less than 3, return 1
    if(tokens.size() < 3) { return 1; }
	
    for(int i = 0; i < tokens.size() - 2; i++) {
        QGram qgram(tokens[i], tokens[i+1], tokens[i+2]);
        _qgramSet.insert(qgram);
    }
    return 0;
}

int Predictor::computeFeature(int64_t& time) {
  time = currentTime();
    int ret = 0;
    int width = _qgrams.size();
    Flowval* feature = new Flowval(width);
    QGramSet::iterator iter = _qgramSet.begin();
    for(; iter != _qgramSet.end(); iter++) {
        const QGram& qgram = *iter;
        if(_qgrams.count(qgram)) {
            Flowval* flowval = _qgrams[qgram];
            feature->flOr(flowval);
        } else {
            // how many qgrams are not in the qgams_map
            ++ret;
            cout<<"--miss--"<<*iter<<endl;
        }
    }
    
    if(ret) {  //there are missing qgrams
        cout<<"warning: there are "<<ret<<" missing qgrams"<<endl;
    }
    //output feature
    time = currentTime() - time;
    outputFeature(feature);
}

void Predictor::outputFeature(Flowval* feature) {
    string filename = sDir + _fbase +".test";
    ofstream fout(filename.c_str());
    fout<<1;
    for (int i = 0; i < feature->size(); i++) {
        if(feature->value(i)) {
            fout<<" "<<i+1<<":"<<"1";
        } else {
            fout<<" "<<i+1<<":"<<"0";
        }
    }
    fout<<endl;
    fout.close();
}

int Predictor::predict(int64_t& svmTime) {
    svm(_fbase, svmTime);
    string pfile = sDir + _fbase +".predict";
    ifstream fin(pfile.c_str());
    string line;
    if(fin.eof()) { return 0; }
    getline(fin, line);
    if(line == "1") {
        cout<<"good"<<endl;
        return FGood;
    } else if(line == "-1") {
        cout<<"malicious"<<endl;
        return FMali;
    }
    return 0;
}


}

