#ifndef SHREK_PREDICTOR_H
#define SHREK_PREDICTOR_H

#include <Common.h>
#include <QGramsReader.h>
namespace shrek {

class Predictor
{
public:
    Predictor(QGramMap& qgrams) 
        : _qgrams(qgrams) {}
    ~Predictor() {}
private:
    Predictor(const Predictor &);
    Predictor& operator=(const Predictor &);
public:
    QGramSet _qgramSet;
    QGramMap& _qgrams;
    std::string _fbase;
    static const std::string sDir;
public:
    int parseTokens(std::string& fbase);
    int computeFeature(int64_t& time);
    void outputFeature(Flowval* feature);
    int predict(int64_t& svmTime);
};

SHREK_TYPEDEF_PTR(Predictor);

}

#endif //SHREK_PREDICTOR_H
