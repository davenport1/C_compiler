/*
* file: Symbol.h
*
*/

#ifndef SYMBOL_H
#define SYMBOL_H
#include "Type.h"
#include <string>

//abbreviate things by using typedef, can do typedef in class

class Symbol {
	typedef std::string string;
	string _name;				//have no type called string (its in std namespace) so use typedef above
	Type _type;

public:
	//constructor
	Symbol(const string &name, const Type & type);	//we always pass objects this way using const and by reference
	//accessors
	const string &name() const;			//const, accessor shouldn't modify anything
			//anytime returning a large object via accessor, either return a copy, or if you want to 
			//		 return a ref, then declare it as const so it cannot be changed
			// (const string &name) <-- return type
			// const at the end is the name of this so: const Symbol *this;
	const Type &type() const; 
	
	
};	
	
#endif

