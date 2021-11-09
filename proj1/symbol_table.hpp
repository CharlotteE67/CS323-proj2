#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE


#include <string>
#include <iostream>
#include <vector>
#include <map>
#include "spl_node.hpp"
#include "spl_type.hpp"

using namespace std;

FieldList* defVisit(Node *def,bool flagR);

void extDef_SES(Node *def);

void checkFuncReturn(Node *extDef);

void structDec(Node *def);

void semanticErrors(int typeID, int lineNo);

bool isMatchedType(Type *t1, Type *t2);

void checkVarDef(Node *id);

void funcDec(Node *exDef);

void checkRvalueOnLeft(Node *left, int lineNum);

void checkAssignOp(Node *left, Node *right, Node *parent, int lineNum);

void checkBoolOp(Node *left, Node *right, Node *parent, int lineNum);

void checkMathOp(Node *left, Node *right, Node *parent, int lineNum);

#endif