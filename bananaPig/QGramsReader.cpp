#include <QGramsReader.h>

using namespace std;
using namespace std::tr1;
namespace shrek {

QGramsReader::QGramsReader() { 
}

QGramsReader::~QGramsReader() { 
}

void QGramsReader::operator()(string filename) {
    ifstream fin(filename.c_str());
    string line;
    while(fin) {
        if(fin.eof()) break;
        getline(fin, line);
        if(line.empty()) { continue; }
        QGram qgram;
        qgram.init(line);
        _qgrams[qgram] = NULL;
    }
    fin.close();

    int size = _qgrams.size();
    QGramMap::iterator iter = _qgrams.begin();
    int i = 0;
    for(; iter != _qgrams.end(); ++iter) {
        Flowval* flowval = new Flowval(size);
        flowval->set(i);
        iter->second = flowval;
        ++i;
    }

}

}

