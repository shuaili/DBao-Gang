#ifndef QGRAM_H
#define QGRAM_H
#include <Common.h>
class QGram
{
public:
    std::string _str1;
    std::string _str2;
    std::string _str3;

    QGram() {}
    QGram(std::string& str1, std::string& str2, std::string& str3) {
        _str1 = str1;
        _str2 = str2;
        _str3 = str3;
    }

    ~QGram(){}

    void init(std::string line) {
        int i = 0, idx = 0;
        std::string temp[3];
        while(i < line.size()) {
            int start = i;
            while (!isspace(line[i]) && i < line.size()) { i++; }
            int len = i - start;
            if(len) {
                std::string str = line.substr(start, len);
                temp[idx] = str;
                idx++;
            } else { //fix bug: there are whitespace at the begining of the file
                break;
            }
            while (isspace(line[i]) && i< line.size()) { i++; }
        }
        assert(idx <= 3);
        _str1 = temp[0];
        _str2 = temp[1];
        _str3 = temp[2];
    }

    QGram & operator = (QGram &rhs) {
        if (this != &rhs) {
            _str1 = rhs._str1;
            _str2 = rhs._str2;
            _str3 = rhs._str3;
        }
        return *this;
    }
/*
    QGram(const QGram &other) {
        *this = other;
    }
*/
    friend bool operator == (const QGram & a, const QGram & b) {
        return a._str1 == b._str1 
            && a._str2 == b._str2 
            && a._str3 == b._str3;
    }

    friend bool operator > (const QGram & a,const QGram & b) {
        return a._str1 > b._str1
            || (a._str1 == b._str1 && a._str2 > b._str2)
            || (a._str1 == b._str1 && a._str2 == b._str2 && a._str3 > b._str3);
    }

    friend bool operator < (const QGram &a, const QGram & b) {
        return !(a > b || a == b);
    }

    friend std::ostream & operator << (std::ostream & out, const QGram &a ) {
        return out<<a._str1<<" "<<a._str2<<" "<<a._str3<<std::endl;
    }
};

#endif // QGRAM_H
