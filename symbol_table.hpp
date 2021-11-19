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

void semanticErrors(int typeID, int lineNo, string arg1 = "", string arg2 = "");

bool isMatchedType(Type *t1, Type *t2);

void checkVarDef(Node *id, Node *parent);

void checkStructDot(Node *exp);

void funcDec(Node *exDef);

void funcArgDec(Node *varList);

FieldList* getFunDecArgs(Node *varList);
FieldList* getFunCallArgs(Node *args);

void checkRvalueOnLeft(Node *left, int lineNum);
void checkFuncCall(Node *root, Node *id, Node *args);


void checkRvalueOnLeft(Node *left);

void checkAssignOp(Node *left, Node *right, Node *parent);

void checkBoolOp(Node *left, Node *right, Node *parent);

void checkCompOp(Node *left, Node *right, Node *parent);

void checkMathOp(Node *left, Node *right, Node *parent);

void checkIndexType(Node *arr, Node *index);
void checkIndexBound(Node *arr, Node *index);

void checkArrayType(Node *root, Node *node);

#endif