/**
 * @file checker.cpp
 * 
 * 
 * WHATS NOT WORKING: 
 *      undeclared              (check identifier)
 *      type void               (check identifier (?))
 *      redefining functions    (defineFunction checking redef)
 */

# include "checker.h"
# include <iostream>
# include "Scope.h"
# include "lexer.h"
# include "tokens.h"

using namespace std;

static Scope* global = nullptr;
static Scope* current = nullptr;

static string E1 = "redefinition of '%s'";
static string E2 = "conflicting types for '%s'";
static string E3 = "redeclaration of '%s'";
static string E4 = "'%s' undeclared";
static string E5 = "'%s' has type void";

//open a new scope and place on stack, if global scope
//is a nullptr then the opening scope is global
void openScope() {
    cout << "open scope" << endl;
    current = new Scope(current);
    if(global == nullptr) {
        global = current;
    }

}

// set current scope back to its enclosing scope and free memory
// taken by previous scope (?)
void closeScope() {
    cout << "close scope" << endl;
    Scope* temp = current;
    current = current->enclosing_scope();
    delete(temp);
}

//check for redefinition of function
void defineFunction(const std::string &name, const Type &type) {
    Symbol *symbol = global->find(name);
    //check if function already declared
    if(symbol ==nullptr) {
        global->insert(new Symbol(name,type));
    }
    //check for conflicting declarations
    else if(symbol->type() != type) {
        report(E3, name);
    }
    //function delcaration exists, replace with new identical declaration
    // else {
    //     global->remove(name);
    //     global->insert(new Symbol(name, type));
    // }
    //checking for redefinition
    else if (type.parameters() != nullptr) {
        report(E1, name);
    }
    cout << "define: " << name << ": " << type << endl;

}

// check for redeclaration(types parameters are )
void declareFunction(const std::string &name, const Type &type) {
    //check we are in global scope
    Symbol *symbol = global->find(name);
    if(symbol == nullptr) {
        global->insert(new Symbol(name, type));
    }
    else if(symbol->type() != type){
        //check if function declarations equal to each other if 
        //symbol for function name is found
        report(E2, name);
    }
    //function delcaration exists, replace with new identical declaration
    // else {
    //     global->remove(name);
    //     global->insert(new Symbol(name, type));
    // }
    cout << "declare: " << name << ": " << type << endl;
}


void declareVariable(const std::string &name, const Type &type) {
    cout << name << ": " << type << endl;
    Symbol *symbol = current->find(name);
    if(symbol == nullptr) {
        if(type.specifier() == VOID && type.indirection() == 0) {
            report(E5, name);
            return;
        }
        current->insert(new Symbol(name,type));
    }
    else if(current->enclosing_scope() != nullptr) {
        report(E3, name);
        //do not put string here, make variable for string
    }
    else if(symbol->type() != type) {
        //types are different, how do we check? 
        //we overloaded != operator for type
        //then error
        report(E2, name);

    }
    //else we do nothing

    //somewhere put const string redeclaration = "redeclared blah blah"

}


void checkIdentifier(const std::string &name) {
    cout << "checkIdentifier" << name << endl;
    if(current->lookup(name) == nullptr)
        report(E4, name);
}