/**
 * file: Scope.h
 *  header file for scope class
 *  linked list of scopes pointing to each other
 *  each global scope as tail
 *  each scope has a vector of symbol pointers defined in that scope
 *  next pointer for each scope points to the enclosing scope
*/

#include "Scope.h"

using namespace std;

Scope::Scope(Scope *enclosing_scope)
    :_enclosing_scope(enclosing_scope)
{

}

/* insert a symbol */
void Scope::insert(Symbol *symbol)
{
    _symbols.push_back(symbol);
}

/* remove a symbol */
void Scope::remove(const std::string &name)
{
    symbolsVector::iterator it = _symbols.begin();
    while(it != _symbols.end()) {
        if((*it)->name() == name) {
            _symbols.erase(it);
            break;
        }
        it++;
    }
    // it = std::find(_symbols.begin(), _symbols.end(), name);
    // _symbols.erase(std::remove(_symbols.begin(), _symbols.end(), name), _symbols.end());
    
}

/* find and return a symbol with a 
    given name ONLY in the current scope */
Symbol *Scope::find(const std::string &name) const
{
    //iterate through symbols vector and return if found
    for(unsigned i = 0; i < _symbols.size(); i++) {
        if(_symbols[i]->name() == name) {
            return _symbols[i];
        }
    }
    // return nullptr if symbol not found in current scope
    return nullptr;
}

/* find and return nearest symbol with a given name 
    starting the search in the given scope and moving 
    into the enclosing scopes ONLY if you didn’t find it yet! */
Symbol *Scope::lookup(const std::string &name) const
{
    Symbol *symbol = find(name);

    //check in current scope
    if(symbol != nullptr) {
        return symbol; //symbol found
    }

    //iterate through nesting scopes
    if(_enclosing_scope != nullptr) {
        return _enclosing_scope->lookup(name);
    }

    //fall through, couldn't find so return nullptr
    return nullptr;
}
