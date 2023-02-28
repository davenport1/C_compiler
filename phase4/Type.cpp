/*
 * File:	Type.cpp
 *
 * Description:	This file contains the member function definitions for
 *		types in Simple C.  A type is either a scalar type, an
 *		array type, or a function type.
 *
 *		Note that we simply don't like putting function definitions
 *		in the header file.  The header file is for the interface.
 *		Actually, we prefer opaque pointer types in C where you
 *		don't even get to see what's inside, much less touch it.
 *		But, C++ lets us have value types with access control
 *		instead of just always using pointer types.
 *
 *		Extra functionality:
 *		- equality and inequality operators
 *		- predicate functions such as isArray()
 *		- stream operator
 *		- the error type
 */

# include <cassert>
# include "tokens.h"
# include "Type.h"

using namespace std;


/*
 * Function:	Type::Type (constructor)
 *
 * Description:	Initialize this type as an error type.
 */

Type::Type()
    : _declarator(ERROR)
{
}


/*
 * Function:	Type::Type (constructor)
 *
 * Description:	Initialize this type object as a scalar type.
 */

Type::Type(int specifier, unsigned indirection)
    : _specifier(specifier), _indirection(indirection), _declarator(SCALAR)
{
}


/*
 * Function:	Type::Type (constructor)
 *
 * Description:	Initialize this type object as an array type.
 */

Type::Type(int specifier, unsigned indirection, unsigned long length)
    : _specifier(specifier), _indirection(indirection), _length(length)
{
    _declarator = ARRAY;
}


/*
 * Function:	Type::Type (constructor)
 *
 * Description:	Initialize this type object as a function type.
 */

Type::Type(int specifier, unsigned indirection, Parameters *parameters)
    : _specifier(specifier), _indirection(indirection), _parameters(parameters)
{
    _declarator = FUNCTION;
}


/*
 * Function:	Type::operator ==
 *
 * Description:	Return whether another type is equal to this type.  The
 *		parameter lists are checked for function types, which C++
 *		makes so easy.  (At least, it makes something easy!)
 */

bool Type::operator ==(const Type &rhs) const
{
    if (_declarator != rhs._declarator)
	return false;

    if (_declarator == ERROR)
	return true;

    if (_specifier != rhs._specifier)
	return false;

    if (_indirection != rhs._indirection)
	return false;

    if (_declarator == SCALAR)
	return true;

    if (_declarator == ARRAY)
	return _length == rhs._length;

    if (!_parameters || !rhs._parameters)
	return true;

    return *_parameters == *rhs._parameters;
}


/*
 * Function:	Type::operator !=
 *
 * Description:	Well, what do you think it does?  Why can't the language
 *		generate this function for us?  Because they think we want
 *		it to do something else?  Yeah, like that'd be a good idea.
 */

bool Type::operator !=(const Type &rhs) const
{
    return !operator ==(rhs);
}


/*
 * Function:	Type::isArray
 *
 * Description:	Return whether this type is an array type.
 */

bool Type::isArray() const
{
    return _declarator == ARRAY;
}


/*
 * Function:	Type::isScalar
 *
 * Description:	Return whether this type is a scalar type.
 */

bool Type::isScalar() const
{
    return _declarator == SCALAR;
}


/*
 * Function:	Type::isFunction
 *
 * Description:	Return whether this type is a function type.
 */

bool Type::isFunction() const
{
    return _declarator == FUNCTION;
}


/*
 * Function:	Type::isError
 *
 * Description:	Return whether this type is an error type.
 */

bool Type::isError() const
{
    return _declarator == ERROR;
}


/*
 * Function:	Type::specifier (accessor)
 *
 * Description:	Return the specifier of this type.
 */

int Type::specifier() const
{
    return _specifier;
}


/*
 * Function:	Type::indirection (accessor)
 *
 * Description:	Return the number of levels of indirection of this type.
 */

unsigned Type::indirection() const
{
    return _indirection;
}


/*
 * Function:	Type::length (accessor)
 *
 * Description:	Return the length of this type, which must be an array
 *		type.  Is there a better way than calling assert?  There
 *		certainly isn't an easier way.
 */

unsigned long Type::length() const
{
    assert(_declarator == ARRAY);
    return _length;
}


/*
 * Function:	Type::parameters (accessor)
 *
 * Description:	Return the parameters of this type, which must be a
 *		function type.
 */

Parameters *Type::parameters() const
{
    assert(_declarator == FUNCTION);
    return _parameters;
}

/**
 * Functions: isPredicate
 * 
 * Description: Determines if current type is a predicate 
 *      or not
*/
bool Type::isPredicate() const 
{
    return(this->isPointer() || this ->isNumeric());

}

/**
 * Function: isPointer
 * 
 * Description: Determines if current type is a pointer or
 *      not, returns boolean
*/
bool Type::isPointer() const
{
    return (_declarator == SCALAR && _indirection > 0) || _declarator == ARRAY;

}

/**
 * Function: isNumeric
 * 
 * Description: Determines if current type is a numeric 
 *      value or not, returning boolean
*/
bool Type::isNumeric() const
{
    return (_declarator == SCALAR && _indirection == 0 && _specifier != VOID);
}

/**
 * function: isCompatibleWith
 * 
 * Two types are compatible if (after any promotion) they
 *      are both numeric, both are “pointer to T”, where T is identical, 
 *      or if one is “pointer to T” and the other is “pointer
 *      to void
*/
bool Type::isCompatibleWith(const Type &that) const {
    Type this_promoted = this->promote();
    Type that_promoted = that.promote();
    if(this_promoted.isNumeric() && that_promoted.isNumeric()) {
        return true;
    }
    if(this_promoted.isPointer() && this_promoted == that_promoted) {
        return true;
    }
    if(this_promoted.isPointer() && that_promoted == Type(VOID, 1)) {
        return true;
    }
    if(this_promoted == Type(VOID, 1) && that_promoted.isPointer()) {
        return true;
    }
    
    //do last case separate as well
    //check both pointers
    //one pinter to T other pointer to VOID
    //void pointer and void double pointer are compatible
    //**void not compatible with ***void

    // if(this->isNumeric() && that.isNumeric()) {
    //     if(this->isPointer() && that == Type(VOID, 1)) {
    //         return true;
    //     }
    //     else if(*this == Type(VOID, 1) && that.isPointer()) {
    //         return true;
    //     }
    //     else {
    //         Type this_promoted = this->promote();
    //         Type that_promoted = that.promote();
    //         if(this_promoted == that_promoted) {
    //             return true;
    //         }
    //     }
    // }
    return false;
}

/**
 * Function: promote
 * 
 * returns promoted type if promotion nescessary
 *      char may be promoted to int
 *      array of T may be promoted to pointer to T
*/
Type Type::promote() const
{        
    // promote char to int
    if(_specifier == CHAR && _declarator == SCALAR) {
        return Type(INT);
    }
    // promote array to pointer
    if(_declarator == ARRAY) {
        return Type(_specifier, _indirection + 1);
    }

    return *this;
}

/*
 * Function:	operator <<
 *
 * Description:	Write a type to the specified output stream.  At least C++
 *		let's us do some cool things.
 */

ostream &operator <<(ostream &ostr, const Type &type)
{
    if (type.isError())
	ostr << "error";

    else {
	if (type.specifier() == CHAR)
	    ostr << "char";
	else if (type.specifier() == INT)
	    ostr << "int";
	else if (type.specifier() == LONG)
	    ostr << "long";
	else if (type.specifier() == VOID)
	    ostr << "void";
	else
	    ostr << "unknown";

	if (type.indirection() > 0)
	    ostr << " " << string(type.indirection(), '*');

	if (type.isArray())
	    ostr << "[" << type.length() << "]";

	else if (type.isFunction())
	    ostr << "()";
    }

    return ostr;
}
