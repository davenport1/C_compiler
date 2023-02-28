/**
 * file: Symbol.cpp
*/

#include "Symbol.h"

using namespace std;

//constructor
Symbol::Symbol(const string &name, const Type & type) 
    : _name(name), _type(type)
{

}

//accessors
const string &Symbol::name() const {
    return _name;
}

const Type &Symbol::type() const {
    return _type;
}

