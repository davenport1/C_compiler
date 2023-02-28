/*
 * File:	checker.h
 *
 * Description:	This file contains the public function declarations for the
 *		semantic checker for Simple C.
 */

# ifndef CHECKER_H
# define CHECKER_H
# include "Scope.h"

// static Type integer(INT);
// static Type error(INT);

Scope *openScope();
Scope *closeScope();

Symbol *defineFunction(const std::string &name, const Type &type);
Symbol *declareFunction(const std::string &name, const Type &type);
Symbol *declareVariable(const std::string &name, const Type &type);
Symbol *checkIdentifier(const std::string &name);

Type checkLogical(const Type &left, const Type &right, const std::string &op);
Type checkNot(const Type &right);
Type checkMultiplicative(const Type &left, const Type &right, const std::string &op);
Type checkNeg(const Type &right);
Type checkRelational(const Type &left, const Type &right, const std::string &op);
Type checkEquality(const Type& left, const Type &right, const std::string &op);
Type checkSub(const Type &left, const Type &right);
Type checkAdd(const Type &left, const Type &right);
Type checkDeref(const Type &right);
Type checkPost(const Type &left, const Type &right);
Type checkAddr(const Type &right, const bool &lvalue);
Type checkSizeof(const Type& right);

Type checkIf(const Type &left);
Type checkFor(const Type &left);
Type checkWhile(const Type &left);
Type checkReturn(const Type &func, const Type &right);

Type checkFunction(const Type &left, Parameters *params);
Type checkAssignment(const Type &left, const Type &right, const bool &lvalue);


# endif /* CHECKER_H */
