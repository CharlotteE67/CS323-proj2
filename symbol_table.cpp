#include "symbol_table.hpp"

map<string, Type *> symbolTable;
static map<string, Type *> strToType = {};
static map<Type *, string> typeToStr = {};

/* def -> specifier -> type */
string defGetTypeName(Node *def) {
    Node *type = def->child[0]->child[0];
    return type->get_name();
}

/* def -> Specifier -> StructSpecifier -> STRUCT ID LC DefList RC | STRUCT ID */
string defGetStructName(Node *def) {
    Node *stuID = def->child[0]->child[0]->child[1];
    return stuID->get_name();
}

/* VarDec:ID
*    | VarDec LB INT RB
*/
string vardecGetName(Node *vardec) {
    while (vardec->child.size() != 1) {
        vardec = vardec->child[0];
    }
    return vardec->child[0]->get_name();
}

/* DecList -> Dec
        | Dec COMMA DecList
    Dec -> VarDec
        | VarDec ASSIGN Exp */
Node *decListGetVarDec(Node *decList) {
    return decList->child[0]->child[0];
}

FieldList *defPrimitiveType(Node *def, Type *outlayer) {
    Node *decList = def->child[1];
    string dataType = defGetTypeName(def);
    FieldList *head = new FieldList();
    FieldList *ptr = head;

    while (true) {
        Node *dec = decList->child[0];
        Node *varDec = decListGetVarDec(decList);
        string varName = vardecGetName(varDec);

        if (outlayer!=nullptr) {
            //struct need, return fieldList
            FieldList *next = new FieldList(varName, symbolTable[varName], nullptr);
            symbolTable[varName]->set_typePtr(outlayer);
            ptr->next = next;
            ptr = ptr->next;
        } else {
            if (symbolTable.count(varName) == 1) {
                //check data type
                semanticErrors(3, def->get_lineNo());
            }
            if (varDec->child.size() == 1) {
                //vardec -> id
                symbolTable[varName] = new Type(varName, dataType);

            } else {
                //array type
                Type *base = new Type(varName, dataType);
                while (varDec->child.size() != 1) {
                    int arrSize = varDec->child[2]->get_intVal();
                    Array *arr = new Array(base, arrSize);
                    Type *upper = new Type(varName, arr);
                    base = upper;
                    varDec = varDec->child[0];
                }
                symbolTable[varName] = base;
            }
            if (dec->child.size() == 3){

                if (!isMatchedType(symbolTable[varName], dec->child[2]->get_varType())) {
                    semanticErrors(5, varDec->get_lineNo());
                }
                // child[0].get_varType()== child[2].get_varType() ?
            }
        }

        if (decList->child.size() == 1) { break; }
        decList = decList->child[2];
    }
    return head->next;

}

FieldList *defStructType(Node *def, Type *outlayer) {
    Node *decList = def->child[1];
    string structName = defGetStructName(def);

    FieldList *head = new FieldList();
    FieldList *ptr = head;

    while (true) {
        Node *varDec = decListGetVarDec(decList);
        string varName = vardecGetName(varDec);
        if (outlayer!=nullptr) {
            //struct need, return fieldList
            symbolTable[varName]->set_typePtr(outlayer);
            FieldList *next = new FieldList(varName, symbolTable[varName], nullptr);
            ptr->next = next;
            ptr = ptr->next;
        } else {
            if (symbolTable.count(varName) == 1) {
                //redefine
                semanticErrors(3, def->get_lineNo());
            }
            if (symbolTable.count(structName) == 0) {
                //struct undefined
                semanticErrors(14, def->get_lineNo());
            }
            if (varDec->child.size() == 1) {
                //vardec -> id
                symbolTable[varName] = symbolTable[structName];
            } else {
                //array type
                Type *base = symbolTable[structName];
                while (varDec->child.size() != 1) {
                    int arrSize = varDec->child[2]->get_intVal();
                    Array *arr = new Array(base, arrSize);
                    Type *upper = new Type(varName, arr);
                    base = upper;
                    varDec = varDec->child[0];
                }
                symbolTable[varName] = base;
            }
        }

        if (decList->child.size() == 1) { break; }
        decList = decList->child[2];
    }
    return head->next;

}

/* Def -> enter
    local var
*/
FieldList *defVisit(Node *def, Type *outlayer) {
    if (defGetTypeName(def) == "StructSpecifier") {
        //struct
        return defStructType(def, outlayer);
    } else {
        return defPrimitiveType(def, outlayer);
    }
}


/* ExtDef -> Specifier ExtDecList SEMI   (var declaration)
   ExtDecList -> VarDec
        | VarDec COMMA ExtDecList */
void extDef_SES(Node *def) {
    Node *extDec = def->child[1];
    // string dataType = defGetTypeName(def);
    while (true) {
        Node *varDec = extDec->child[0];
        string varName = vardecGetName(varDec);
        Node *type = def->child[0]->child[0];
        Type *ptr;

        if (symbolTable.count(varName) == 1) {
            //redefine
            semanticErrors(3, def->get_lineNo());
        }
        if (type->child.size() == 0) {
            ptr = new Type(varName, type->get_name());
        } else {
            ptr = symbolTable[type->child[1]->get_name()];
        }

        if (varDec->child.size() == 1) {
            symbolTable[varName] = ptr;
        } else {
            //array type
            Type *base = ptr;
            while (varDec->child.size() != 1) {
                int arrSize = varDec->child[2]->get_intVal();
                Array *arr = new Array(base, arrSize);
                Type *upper = new Type(varName, arr);
                base = upper;
                varDec = varDec->child[0];
            }
            symbolTable[varName] = base;
        }
        if (extDec->child.size() == 1) { break; }
        extDec = extDec->child[2];
    }

}

/*
 *  ExtDef:
 *      | Specifier FunDec CompSt
 *  FunDec:
 *      ID ...
 *  CompSt:
 *      LC DefList StmtList RC
 *  StmtList:
 *      NULL
 *      | Stmt StmtList
 *  Stmt:
 *      | RETURN Exp SEMI
 */
std::ostream &operator<<(std::ostream &os, const Primitive &ec) {
    os << static_cast<std::underlying_type<Primitive>::type>(ec);
    return os;
}

string getPriType(Primitive pri){
    switch (pri) {
        case Primitive::INT:
            return "int";
            break;
        case Primitive::FLOAT:
            return "float";
            break;
        case Primitive::CHAR:
            return "char";
            break;
    }
}

string getCatType(CATEGORY cat){
    switch (cat) {
        case CATEGORY::PRIMITIVE:
            return "primitive";
            break;
        case CATEGORY::ARRAY:
            return "array";
            break;
        case CATEGORY::STRUCTURE:
            return "struct";
            break;
    }
}

void dfsCheckReturn(Node *root, Type *type) {
    if (root == nullptr || root->child.empty()) return;
    if (root->child[0]->get_name() == "RETURN") {
        if (!isMatchedType(type, root->child[1]->get_varType())) {
            semanticErrors(8, root->get_lineNo());
        }
        return;
    }

    for (auto ch: root->child) {
        dfsCheckReturn(ch, type);
    }
}

void checkFuncReturn(Node *extDef) {
    Node *stmtList = extDef->child[2]->child[2];
    Type *deft;
    deft = symbolTable[extDef->child[1]->child[0]->get_name()];

    dfsCheckReturn(stmtList, deft);
}

/*  structure definition
    whenever meet SSP -> STRUCT ID {DefList}
    enter this 
*/
void structDec(Node *ssp) {
    string stuName = ssp->child[1]->get_name();
    FieldList *head = new FieldList();
    FieldList *ptr = head;
    Node *defL = ssp->child[3];

    Type *stu = new Type(stuName,CATEGORY::STRUCTURE);


    if (symbolTable.count(stuName) != 0) {
        //struct redefine
        semanticErrors(15, ssp->get_lineNo());
    } else {
        symbolTable[stuName] = stu;
    }

    while (!defL->child.empty()) {
        Node *def = defL->child[0];
        //def visit
        FieldList *defStu = defVisit(def, stu);
        while (ptr->next != nullptr) {
            ptr = ptr->next;
        }
        ptr->next = defStu;
        defL = defL->child[1];
    }

    stu->set_fieldList(head->next);
    // Type *stu = new Type(stuName, head->next);
    // symbolTable[stuName] = stu;

}

/**
 * VarList -> ParamDec COMMA VarList| ParamDec
 * ParamDec -> Specifier VarDec
 * VarDec -> ID | VarDec LB INT RB
 * 
 * load func args into symbol table.
 * may return FieldList*
 */
void funcArgDec(Node *varList) {

    while (true) {
        Node *paraD = varList->child[0];

        Node *varDec = paraD->child[1];
        Node *type = paraD->child[0]->child[0];
        Type *ptr;
        string varName = vardecGetName(varDec);
        if (symbolTable.count(varName) == 1) {
            semanticErrors(3, varList->get_lineNo());
        }
        if (type->child.size() == 0) {
            ptr = new Type(varName, type->get_name());
        } else {
            ptr = symbolTable[type->child[1]->get_name()];
        }

        if (varDec->child.size() == 1) {
            symbolTable[varName] = ptr;
        } else {
            Type *base = ptr;
            while (varDec->child.size() != 1) {
                int arrSize = varDec->child[2]->get_intVal();
                Array *arr = new Array(base, arrSize);
                Type *upper = new Type(varName, arr);
                base = upper;
                varDec = varDec->child[0];
            }
            symbolTable[varName] = base;
        }

        if (varList->child.size() == 1) { break; }
        varList = varList->child[2];
    }

}

/*
*   ExtDef -> Specifier FunDec CompSt
*   load new func into symbol table
*/
void funcDec(Node *exDef) {
    string funcName = exDef->child[1]->child[0]->get_name();
    string returnName;
    if (symbolTable.count(funcName) != 0) {
        //func redifined
        semanticErrors(4, exDef->get_lineNo());
        return;
    }

    if (defGetTypeName(exDef) == "StructSpecifier") {
        //struct
        returnName = defGetStructName(exDef);
        symbolTable[funcName] = symbolTable[returnName];
    } else {
        returnName = defGetTypeName(exDef);
        symbolTable[funcName] = new Type(funcName, returnName);
    }

    //function arg

    checkFuncReturn(exDef);


}

/*
*   check type 1: undefined var
*   invoked whenever meet EXP->ID
*/
void checkVarDef(Node *id, Node *parent) {
    string name = id->get_name();
    if (symbolTable.count(name) == 0) {
        semanticErrors(1, id->get_lineNo());
        // set null
        parent->set_varType(nullptr);
    } else{
        parent->set_varType(symbolTable[name]);
    }
}

/**
*   check type 13: accessing members of a non-structure variable.
*   EXP -> EXP DOT ID
*/
void checkStructDot(Node *exp) {
    Type *base = exp->child[0]->get_varType();
    Node * id = exp->child[2];
    string subName = id->get_name();
    //check valid
    if(symbolTable.count(subName)==0||base!=symbolTable[subName]->get_typePtr()){
        semanticErrors(13,exp->get_lineNo());
    }else{
        exp->set_varType(symbolTable[subName]);
    }
}

/* Exp -> ID LP Args RP | ID LP RP */
void checkFuncNoDef(Node *root, Node *node) {
    if (symbolTable.count(node->get_name()) == 0) {
        semanticErrors(2, node->get_lineNo());
    }
    root->set_varType(symbolTable[node->get_name()]);
}


void checkRvalueOnLeft(Node *left) {
    // single ID
    if (left->child.size() == 1 && left->child[0]->get_type() == Node_TYPE::ID) {
        return;
    }
    // Exp.ID
    if (left->child.size() == 3 && left->child[0]->get_name() == "Exp" &&
        left->child[1]->get_name() == "DOT" && left->child[2]->get_type() == Node_TYPE::ID) {
        return;
    }
    // with bracket
    if (left->child.size() == 4 && left->child[0]->get_name() == "Exp" && left->child[1]->get_name() == "LB"
        && left->child[2]->get_name() == "Exp" && left->child[3]->get_name() == "RB") {
        return;
    }
    semanticErrors(6, left->get_lineNo());
}

/* Exp -> Exp ASSIGN Exp */
void checkAssignOp(Node *left, Node *right, Node *parent) {
    if (left->get_varType() == nullptr || right->get_varType() == nullptr) {
        parent->set_varType(nullptr);
        return;
    }
    if (!isMatchedType(left->get_varType(), right->get_varType())) {
        semanticErrors(5, left->get_lineNo());
    }
    // assign type to parent
    parent->set_varType(left->get_varType());
}

/* Exp -> Exp AND Exp
       -> Exp OR Exp */
void checkBoolOp(Node *left, Node *right, Node *parent) {
    if (left->get_varType() == nullptr || right->get_varType() == nullptr) {
        parent->set_varType(nullptr);
        return;
    }
    if (left->get_type() != Node_TYPE::INT || right->get_type() != Node_TYPE::INT) {
        semanticErrors(7, left->get_lineNo());
    }
    parent->set_varType(new Type(parent->get_name(), "int"));
}

/* Exp -> | Exp LT Exp | Exp LE Exp | Exp GT Exp
    | Exp GE Exp | Exp NE Exp | Exp EQ Exp */
void checkCompOp(Node *left, Node *right, Node *parent) {
    if (left->get_varType() == nullptr || right->get_varType() == nullptr) {
        parent->set_varType(nullptr);
        return;
    }
    // int float
    if ((left->get_type() != Node_TYPE::INT && left->get_type() != Node_TYPE::FLOAT)
        || (right->get_type() != Node_TYPE::INT && right->get_type() != Node_TYPE::FLOAT)) {
        semanticErrors(7, left->get_lineNo());
    }
    // assign type to parent
    parent->set_varType(new Type(parent->get_name(), "int"));
}

/* Exp ->  Exp PLUS Exp | Exp MINUS Exp | Exp MUL Exp | Exp DIV Exp
          | MINUS Exp %prec UMINUS */
void checkMathOp(Node *left, Node *right, Node *parent) {
    if (left->get_varType() == nullptr || right->get_varType() == nullptr) {
        parent->set_varType(nullptr);
        return;
    }
    if ((left->get_type() == Node_TYPE::INT && right->get_type() == Node_TYPE::INT)
        || (left->get_type() == Node_TYPE::FLOAT && right->get_type() == Node_TYPE::FLOAT)) {
        parent->set_varType(left->get_varType());
    } else {
        semanticErrors(7, left->get_lineNo());
        parent->set_varType(nullptr);
    }
//    if (left->get_type() == Node_TYPE::FLOAT){
//        parent->set_varType(left->get_varType());
//    } else if (right->get_type() == Node_TYPE::FLOAT) {
//        parent->set_varType(right->get_type());
//    } else{
//        parent->set_varType(left->get_varType());
//    }
    // assign type to parent
//    parent->set_varType(left->get_varType());
}

/* Exp -> Exp LB Exp RB */
void checkIndexType(Node *index) {
    if (index->get_type() != Node_TYPE::INT)
        semanticErrors(12, index->get_lineNo());
}

/* Exp -> Exp LB Exp RB */
void checkArrayType(Node *root, Node *node) {
//    Type *type = symbolTable[node->child[0]->get_name()];
//    if (type->category != CATEGORY::ARRAY)
//        semanticErrors(10, node->get_lineNo());
//    root->set_varType(type->);
}

void semanticErrors(int typeID, int lineNo) {
    switch (typeID) {
        case 1:
            printf("Error type 1 at Line %d: undefined variable.\n", lineNo);//done
            break;
        case 2:
            printf("Error type 2 at Line %d: undefined function.\n", lineNo);//done
            break;
        case 3:
            printf("Error type 3 at Line %d: variable redefined.\n", lineNo);//done
            break;
        case 4:
            printf("Error type 4 at Line %d: function redefined.\n", lineNo);//done
            break;
        case 5:
            printf("Error type 5 at Line %d: unmatching types appear at both sides of the assigment operator.\n",
                   lineNo);//done
            break;
        case 6:
            printf("Error type 6 at Line %d: value can not be assigned.\n", lineNo);//done
            break;
        case 7:
            printf("Error type 7 at Line %d: unmatching operands.\n", lineNo);//done
            break;
        case 8:
            printf("Error type 8 at Line %d: function's return value type mismatch.\n", lineNo);
            break;
        case 9:
            printf("Error type 9 at Line %d: function's arguments mismatch.\n", lineNo);
            break;
        case 10:
            printf("Error type 10 at Line %d: applying indexing operator on non-array type variables.\n", lineNo);
            break;
        case 11:
            printf("Error type 11 at Line %d: applying function invocation operator on non-function names.\n", lineNo);
            break;
        case 12:
            printf("Error type 12 at Line %d: array indexing with a non-integer type expression.\n", lineNo);
            break;
        case 13:
            printf("Error type 13 at Line %d: accessing members of a non-structure variable.\n", lineNo);
            break;
        case 14:
            printf("Error type 14 at Line %d: accessing an undefined structure member.\n", lineNo);
            break;
        case 15:
            printf("Error type 15 at Line %d: structure type redefined.\n", lineNo);//done
            break;

        default:
            break;
    }
}

/* check two type matched
 * return a bool value
 */
bool isMatchedType(Type *t1, Type *t2) {
    if (t1 == nullptr || t2 == nullptr) return false;
    if (t1->category != t2->category) return false;
    // todo: structure eq
    if (t1->category == CATEGORY::STRUCTURE) {
        return symbolTable[t1->name]->name == symbolTable[t2->name]->name;
    }
    if (t1->category == CATEGORY::ARRAY) {
        Array *a1 = t1->type.arr, *a2 = t2->type.arr;
        return (a1->size != a2->size) && isMatchedType(a1->base, a2->base);
    }

    return t1->type.pri == t2->type.pri;
}

