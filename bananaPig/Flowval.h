#ifndef SHREK_FLOWVAL_H
#define SHREK_FLOWVAL_H

#include <Common.h>
#include <fstream>

namespace shrek {

class Flowval
{
private:
//   Flowval(const Flowval &);
//    Flowval& operator=(const Flowval &);

public:
    ~Flowval(){}
    Flowval (int bits) {
        _bits = bits;
        int len = (_bits + sBitsPerInt64 - 1) / sBitsPerInt64;
        _bitVec.resize(len, 0);
    }

    Flowval (const Flowval & other) {
        _bits = other._bits;
        _bitVec = other._bitVec;
    }

    Flowval & operator=(Flowval & rhs) {
        if ( this != &rhs ) {
            _bits = rhs._bits;
            _bitVec = rhs._bitVec;
        }
        return *this;
    }

    int size(void) { return _bits; }

    void set (int pos, bool val) {
        int i = pos / sBitsPerInt64;
        int j = pos % sBitsPerInt64;
        if (val) {
            _bitVec[i] |= (1ll<<j);
        } else {
            _bitVec[i] &= ~(1ll<<j);
        }
    }
    void set (int pos) {
        set (pos, true);
    }
    void reset (int pos) {
        set (pos, false);
    }

    bool value (int pos) {
        int i = pos / sBitsPerInt64;
        int j = pos % sBitsPerInt64;
        return _bitVec[i] & (1ll<<j);
    }
/*
    void and (Flowval& other) {
        assert(other._bitVec.size() == _bitVec.size());
        for (int i = 0; i < _bitVec.size(); i++) {
            _bitVec[i] &= other->_bitVec[i];
        }
    }
*/
    void flOr (Flowval* other) {
        assert(other->_bitVec.size() == _bitVec.size());
        for (int i=0; i < _bitVec.size(); i++) 
            _bitVec[i] |= other->_bitVec[i];
    }
/*
    void not (void) {
        for (int i = 0; i < _bitVec.size(); i++)
            _bitVec[i] = ~_bitVec[i];
    }
*/
    void difference(Flowval& other) {
        assert(other._bitVec.size() == _bitVec.size());
        Flowval comp(other);
//        comp.not();
//        and(comp);
    }

    friend bool operator==(Flowval & a, Flowval & b) {
        if ( a._bits != b._bits ) {
	    return false;
        }
        for ( int i = 0; i < a._bitVec.size(); i++ ) {
	    if ( a._bitVec[i] != b._bitVec[i] )
                return false;
        }
        return true;
    }

    friend bool operator!=(Flowval & a, Flowval & b) {
        return ! (a == b);
    }

    Flowval *clone (void) {
        return new Flowval (*this);
    }


    friend std::ostream & operator<<(std::ostream & out, Flowval & a) {
        out << "(" << a.size() << ")" << " ( ";
        for ( int i = 0; i < a.size(); i++ )
	    out << a.value(i);
        out << " )";
        return out;
    }

public:
    const static int sBitsPerInt64 = 64;
    std::vector<int64_t> _bitVec;
    int	_bits;

};

SHREK_TYPEDEF_PTR(Flowval);

}

#endif //SHREK_FLOWVAL_H
