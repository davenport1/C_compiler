/**
 * file: Scope.h
 *  header file for scope class
 *  linked list of scopes pointing to each other
 *  each global scope as tail
 *  each scope has a vector of symbol pointers defined in that scope
 *  next pointer for each scope points to the enclosing scope
*/

#ifndef SCOPE_H
#define SCOPE_H
#include "Symbol.h"
#include <forward_list>




class Scope
{
    typedef std::vector<Symbol*> symbolsVector;
    typedef std::forward_list<symbolsVector> symbol_table;
    /* data */
    symbolsVector _symbols;
    Scope *_enclosing_scope;

public:
    Scope(Scope *enclosing_scope = nullptr);
    Scope* enclosing_scope() const { return _enclosing_scope; }
    const symbolsVector symbols() const { return _symbols; }
    void insert(Symbol *symbol);
    void remove(const std::string &name);
    Symbol *find(const std::string &name) const;
    Symbol *lookup(const std::string &name) const;

};


#endif