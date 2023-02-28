 /**
 * @file Type.cpp
 * 
 */

#include <cassert>
#include "Type.h"

using namespace std;

//need to write constructors and overloaded operators
Type::Type()
{
	
}

Type::Type(int specifier, unsigned indirection)
	:_specifier(specifier),_indirection(indirection),_declarator(SCALAR)  
{
	// don't need anything here 
}

Type::Type(int specifier, unsigned indirection, unsigned long length)
	:_specifier(specifier),_indirection(indirection),_declarator(ARRAY),_length(length)  
{
	// don't need anything here 
}

Type::Type(int specifier, unsigned indirection, Parameters *parameters)
	:_specifier(specifier),_indirection(indirection),_declarator(FUNCTION), _parameters(parameters)
{
	// don't need anything here 
}


bool Type::operator==(const Type &rhs) const {
	// check if things not the same rather than if they are the same, can exit sooner
	if(_declarator != rhs._declarator) 					//don't need accessors, we are member of class
		return false; 		//can't be the same, if this was false, then we know declarator is the same
	if(_declarator == ERROR)
		return true;			//error only has declarator, not specifier/indirection
	if(_specifier != rhs._specifier) 
		return false;
	if(_indirection != rhs._indirection)
		return false;
	//at this point if we haven't returned we know declarator, specifier and indirection are all same
	if(_declarator == SCALAR) 
		return true;	//we know that we have two scalars equal to each other
	if(_declarator == ARRAY) 
		return _length == rhs._length;
	// if we got here we know we have 2 functions
	assert(_declarator == FUNCTION); //include c assert, always good to check this, if this fails we fucked up 
	if( !_parameters || !rhs._parameters) 
		return true; //cant check anything, null pointers, unspecified
	//now know both are functions and parameters functions have to exist, how to check vectors are equal? just use ==
	return *_parameters == *rhs._parameters; //can safely deref since we know they are not null, checked up above. 
}

bool Type::operator!=(const Type &rhs) const { return !operator==(rhs); }

ostream &operator<<(ostream &ostr, const Type &type) {
    ostr << type._declarator << '/' << type._specifier << '/' << type._indirection;
	if(type._declarator == Type::ARRAY) {
		ostr << '/' << type._length;
	}
	// if(type._declarator == FUNCTION) {

	// 	ostr << '/';

	// }
    return ostr;
}
// for <<: 
// return ostr
// do not have access to private members need to use accessors
// 	OR can make them friends
