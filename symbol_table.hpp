#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE


#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include "spl_node.hpp"
#include "spl_type.hpp"

using namespace std;

FieldList* defVisit(Node *def,Type *outlayer);

void structDec(Node *ssp);

void extDef_SES(Node *def);

void checkFuncReturn(Node *extDef);

void structDec(Node *def);

void semanticErrors(int typeID, int lineNo);

bool isMatchedType(Type *t1, Type *t2);

void checkVarDef(Node *id, Node *parent);

void funcDec(Node *exDef);

void funcArgDec(Node *varList);

void checkRvalueOnLeft(Node *left, int lineNum);
void checkFuncNoDef(Node *root, Node *node);

void checkRvalueOnLeft(Node *left);

void checkAssignOp(Node *left, Node *right, Node *parent);

void checkBoolOp(Node *left, Node *right, Node *parent);

void checkMathOp(Node *left, Node *right, Node *parent);

#endif