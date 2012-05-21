#ifndef SHREK_QGRAMSREADER_H
#define SHREK_QGRAMSREADER_H

#include <Common.h>
#include <Flowval.h>
#include <QGram.h>

namespace shrek {
typedef std::map<QGram, Flowval*> QGramMap;
typedef std::set<QGram> QGramSet;

class QGramsReader
{
public:
    QGramsReader();
    ~QGramsReader();
private:
    QGramsReader(const QGramsReader &);
    QGramsReader& operator=(const QGramsReader &);
public:
    QGramMap _qgrams;

    friend std::ostream& operator<<(std::ostream& out, QGramsReader& q) {
        QGramMap::iterator iter = q._qgrams.begin();
        for(; iter != q._qgrams.end(); ++iter) {
            out<<iter->first;
            Flowval& flowval = *(iter->second);
//            out<<flowval<<std::endl;
        }
    }
    void operator()(std::string filename);
};

SHREK_TYPEDEF_PTR(QGramsReader);

}

#endif //SHREK_QGRAMSREADER_H
