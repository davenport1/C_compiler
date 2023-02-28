/**
 * @file Type.h
 * 
 */

#ifndef TYPE_H		//to protect against multiple inclusions
#define TYPE_H		//for example including symbol.h which already includes type.h
#include <vector>
#include <ostream>

typedef std::vector<class Type*> Parameters; //have to specifiy tpye is a class because it hasnt been delcared yet


class Type {
	int _specifier;		//by default everything in class is private
	unsigned _indirection;
	enum { ARRAY, ERROR, FUNCTION, SCALAR } _declarator;	// all of this using these constants are private
	unsigned long _length; //for array length
	Parameters *_parameters;


	public: 
		Type(int specifier, unsigned indirection=0);		//constructor for scalar.. default value goes in declaration not definition
		Type(int specifier, unsigned indirection, unsigned long length); //constructor for array
		Type(int specifier, unsigned indirection, Parameters *parameters); //constructor for function
		Type(); //default constructor for Error, if not defined, you get a default that does nothing


	// at this point you should type make and ensure it works

    bool isArray() const { return _declarator == ARRAY; } //checking if blah blah is array
	// write for function, scalar, etc
	// accessors: 
	int specifier() const { return _specifier; }
	unsigned indirection() const { return _indirection; }
	int declarator() const { return _declarator; }
	unsigned long length() const { return _length; }
	Parameters *parameters() const { return _parameters; }	
	// all the rest of your private variables too
	
	// overload == and != operators for our class: 
	bool operator==(const Type &rhs) const;		//takes one param, other param is what calls the operator
	bool operator!=(const Type &rhs) const;

	//want to overload stream operator (#inclue <ostream>)
	friend std::ostream &operator<<(std::ostream &ostr, const Type &type); // return type is std::ostream returning reference to stream





};

#endif