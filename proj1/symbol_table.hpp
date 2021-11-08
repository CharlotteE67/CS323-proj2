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

void structDec(Node *def);

void semanticErrors(int typeID, int lineNo);

#endif