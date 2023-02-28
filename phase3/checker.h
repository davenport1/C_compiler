/**
 * @file checker.h
 * 
 * 
 */

# include <ostream>
# include "Type.h"

# ifndef CHECKER_H
# define CHECKER_H

void openScope();
void closeScope();

void defineFunction(const std::string &name, const Type &type);
void declareFunction(const std::string &name, const Type &type);
void declareVariable(const std::string &name, const Type &type);
void checkIdentifier(const std::string &name);


# endif