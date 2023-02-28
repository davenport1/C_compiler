/*
 * File:	checker.cpp
 *
 * Description:	This file contains the public and private function and
 *		variable definitions for the semantic checker for Simple C.
 *
 *		If a symbol is redeclared, the redeclaration is discarded
 *		and the original declaration is retained.
 *
 *		Extra functionality:
 *		- inserting an undeclared symbol with the error type
 */

# include <iostream>
# include "lexer.h"
# include "checker.h"
# include "tokens.h"
# include "Symbol.h"
# include "Scope.h"
# include "Type.h"


using namespace std;

static Scope *outermost, *toplevel;
static const Type error;

static string redefined = "redefinition of '%s'";
static string redeclared = "redeclaration of '%s'";
static string conflicting = "conflicting types for '%s'";
static string undeclared = "'%s' undeclared";
static string void_object = "'%s' has type void";


static string E1 = "invalid return type";
static string E2 = "invalid type for test expression";
static string E3 = "lvalue required in expression";
static string E4 = "invalid operands to binary '%s'";
static string E5 = "invalid operand to unary '%s'";
static string E6 = "called object is not a function";
static string E7 = "invalid arguments to called function";


/*
 * Function:	openScope
 *
 * Description:	Create a scope and make it the new top-level scope.
 */

Scope *openScope()
{
    toplevel = new Scope(toplevel);

    if (outermost == nullptr)
	outermost = toplevel;

    return toplevel;
}


/*
 * Function:	closeScope
 *
 * Description:	Remove the top-level scope, and make its enclosing scope
 *		the new top-level scope.
 */

Scope *closeScope()
{
    Scope *old = toplevel;
    toplevel = toplevel->enclosing();
    return old;
}


/*
 * Function:	defineFunction
 *
 * Description:	Define a function with the specified NAME and TYPE.  A
 *		function is always defined in the outermost scope.  This
 *		definition always replaces any previous definition or
 *		declaration.
 */

Symbol *defineFunction(const string &name, const Type &type)
{
    cout << name << ": " << type << endl;
    Symbol *symbol = outermost->find(name);

    if (symbol != nullptr) {
	if (symbol->type().isFunction() && symbol->type().parameters()) {
	    report(redefined, name);
	    delete symbol->type().parameters();

	} else if (type != symbol->type())
	    report(conflicting, name);

	outermost->remove(name);
	delete symbol;
    }

    symbol = new Symbol(name, type);
    outermost->insert(symbol);
    return symbol;
}


/*
 * Function:	declareFunction
 *
 * Description:	Declare a function with the specified NAME and TYPE.  A
 *		function is always declared in the outermost scope.  Any
 *		redeclaration is discarded.
 */

Symbol *declareFunction(const string &name, const Type &type)
{
    cout << name << ": " << type << endl;
    Symbol *symbol = outermost->find(name);

    if (symbol == nullptr) {
	symbol = new Symbol(name, type);
	outermost->insert(symbol);

    } else if (type != symbol->type()) {
	report(conflicting, name);
	delete type.parameters();

    } else
	delete type.parameters();

    return symbol;
}


/*
 * Function:	declareVariable
 *
 * Description:	Declare a variable with the specified NAME and TYPE.  Any
 *		redeclaration is discarded.
 */

Symbol *declareVariable(const string &name, const Type &type)
{
    cout << name << ": " << type << endl;
    Symbol *symbol = toplevel->find(name);

    if (symbol == nullptr) {
	if (type.specifier() == VOID && type.indirection() == 0)
	    report(void_object, name);

	symbol = new Symbol(name, type);
	toplevel->insert(symbol);

    } else if (outermost != toplevel)
	report(redeclared, name);

    else if (type != symbol->type())
	report(conflicting, name);

    return symbol;
}


/*
 * Function:	checkIdentifier
 *
 * Description:	Check if NAME is declared.  If it is undeclared, then
 *		declare it as having the error type in order to eliminate
 *		future error messages.
 */

Symbol *checkIdentifier(const string &name)
{
    Symbol *symbol = toplevel->lookup(name);

    if (symbol == nullptr) {
	report(undeclared, name);
	symbol = new Symbol(name, error);
	toplevel->insert(symbol);
    }

    return symbol;
}

/**
 * Function: checkLogical
 * 
 * Description: Can be used for || or &&, passed operator
 *      makes sure both left and right side of expression
 *      are predicates, returning result Type int
 *      else throwing error invalid operands to binary operator
*/
Type checkLogical(const Type &left, const Type &right, const string &op) {
    if(left == error || right == error) {
        return error;
    }
    Type left_promoted = left.promote();
    Type right_promoted = right.promote();
    if(left_promoted.isPredicate() && right_promoted.isPredicate()) {
        return Type(INT);
    }
    else {
        report(E4, op);
        return error;
    }
}

/**
 * Function: checkPrefix
 * 
 * Description: Checks prefix operators 
*/
Type checkNot(const Type &right) {
    if(right == error) {
        return error;
    }
    if(right.isPredicate()) {
        return Type(INT);
    }
    report(E5, "!");
    return error;

    
    
}

Type checkIf(const Type &left) {
    if(left == error) {
        return error;
    }
    if(left.isPredicate()) {
        return left;
    }
    report(E2);
    return error;
}

Type checkFor(const Type &left) {
    if(left == error) {
        return error;
    }
    if(left.isPredicate()) {
        return left;
    }
    report(E2);
    return error;
}

Type checkWhile(const Type &left) {
    if(left == error) {
        return error;
    }
    if(left.isPredicate()) {
        return left;
    }
    report(E2);
    return error;
}

Type checkReturn(const Type &func, const Type &right) {
    if(func == error || right == error) {
        return error;
    }
    if(right.isCompatibleWith(func)) {
        return right;
    }
    report(E1);
    return error;
}

Type checkMultiplicative(const Type &left, const Type &right, const string &op) {
    if(left == error || right == error) {
        return error;
    }
    if(left.isNumeric() && right.isNumeric()) {
        if(left.specifier() == LONG || right.specifier() == LONG){
            return Type(LONG);
        }
        else {
            return Type(INT);
        }
    }
    report(E4, op);
    return error;
}

Type checkNeg(const Type &right) {
    if(right == error) {
        return error;
    }
    Type right_promoted = right.promote();
    if(right_promoted.isNumeric()) {
        return right_promoted;
    }
    report(E5, "-");
    return error;
}

Type checkRelational(const Type &left, const Type &right, const string &op) {
    if(left == error || right == error) {
        return error;
    }
    Type left_promoted = left.promote();
    Type right_promoted = right.promote();
    if(left_promoted.isNumeric() && right_promoted.isNumeric()) {
        return Type(INT);
    }
    if(left_promoted.isPredicate() && left_promoted == right_promoted) {
        return Type(INT);
    }
    // if(left_promoted == right_promoted && (left_promoted.isNumeric() || left_promoted.isPointer())) {
    //     return Type(INT);
    // }
    report(E4, op);
    return error;
}

Type checkEquality(const Type& left, const Type &right, const string &op) {
    if(left == error || right == error) {
        return error;
    }
    Type left_promoted = left.promote();
    Type right_promoted = right.promote();
    if(left_promoted.isCompatibleWith(right_promoted)) {
        return Type(INT);
    }
    report(E4, op);
    return error;
}

Type checkSub(const Type &left, const Type &right) {
    if(left == error || right == error) {
        return error;
    }
    Type left_promoted = left.promote();
    Type right_promoted = right.promote();
    if(left_promoted.isNumeric() && right_promoted.isNumeric()) {
        if(left_promoted.specifier() == LONG || right_promoted.specifier() == LONG) {
            return Type(LONG);
        }
        else {
            return Type(INT);
        }
    }
    if(left_promoted.isPointer() && right_promoted.isNumeric()) {
        if(left_promoted != Type(VOID, 1)){
            return Type(left_promoted.specifier(), left_promoted.indirection());
        } 
    }
    if(left_promoted.isPointer() && right_promoted.isPointer()) {
        if(left_promoted.isCompatibleWith(right) && left_promoted != Type(VOID, 1)) {
            return Type(LONG);
        }
    }
    report(E4, "-");
    return error;

}

Type checkAdd(const Type &left, const Type &right) {
    if(left == error || right == error) {
        return error;
    }
    Type left_promoted = left.promote();
    Type right_promoted = right.promote();
    if(left_promoted.isNumeric() && right_promoted.isNumeric()) {
        if(left_promoted.specifier() == LONG || right_promoted.specifier() == LONG) {
            return Type(LONG);
        }
        else {
            return Type(INT);
        }
    }
    if(left_promoted.isPointer() && right_promoted.isNumeric()) {
        if(left_promoted != Type(VOID, 1)) {
            return Type(left_promoted.specifier(), left_promoted.indirection());
        } 
    }
    if(left_promoted.isNumeric() && right_promoted.isPointer()) {
        if(right_promoted != Type(VOID, 1)) {
            return Type(right_promoted.specifier(), right_promoted.indirection());
        }
    }
    report(E4, "-");
    return error;
}

Type checkDeref(const Type &right) {
    if(right == error) {
        return error;
    }
    Type right_promoted = right.promote();
    if(right_promoted.isPointer() && right_promoted != Type(VOID, 1)) {
        return Type(right_promoted.specifier(), right_promoted.indirection() - 1);
    }
    report(E5, "*");
    return error;

}

Type checkPost(const Type &left, const Type &right) {
    if(left == error || right == error) {
        return error;
    }
    Type left_promoted = left.promote();
    Type right_promoted = right.promote();
    if(left_promoted.isPointer() && left_promoted != Type(VOID, 1) && right_promoted.isNumeric()) {
        return Type(left_promoted.specifier(), left_promoted.indirection() - 1);
    }
    report(E4, "[]");
    return error;
}

Type checkAddr(const Type &right, const bool &lvalue) {
    if(right == error) {
        return error;
    }
    if(lvalue) {
        return Type(right.specifier(), right.indirection() + 1);
    }
    report(E3);
    return error;
}

Type checkSizeof(const Type &right) {
    if(right == error) {
        return error;
    }
    if(right.isPredicate() && !right.isFunction()) {
        return Type(LONG);
    }
    report(E5, "sizeof");
    return error;
}


Type checkFunction(const Type &left, Parameters *params) {
    if(left == error) {
        return error;
    }
    //left not a function
    if(!left.isFunction()) {
        report(E6);
        return error;
    }
    for(unsigned i = 0; i < params->size(); ++i) {
        Type arg_promoted = params->at(i).promote();
        if(!arg_promoted.isPredicate()) {
            report(E7);
            return error;
        }

    }
    //function declared but not defined, no parameters
    if(left.parameters() == nullptr) {
        return Type(left.specifier(), left.indirection());
    }
    //function declared/defined with parameters
    if(left.parameters()->size() != params->size()) {
        report(E7);
        return error;
    }

    for(unsigned i = 0; i < params->size(); ++i) {
        Type param_promoted = left.parameters()->at(i).promote();
        Type arg_promoted = params->at(i).promote();
        if(!(param_promoted.isCompatibleWith(arg_promoted))) {
            report(E7);
            return error;
        }
        // if(!arg_promoted.isPredicate()) {
        //     report(E7);
        //     return error;
        // }
    }
    return Type(left.specifier(), left.indirection());
}

Type checkAssignment(const Type &left, const Type &right, const bool &lvalue) {
    if(left == error || right == error) {
        return error;
    }
    else if(!lvalue) {
        report(E3);
        return error;
    }
    else if(left.isCompatibleWith(right)) {
        return left;
    }
    report(E4, "=");
    return error;
}