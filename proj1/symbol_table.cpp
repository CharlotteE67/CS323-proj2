#include "symbol_table.hpp"

map<string, Type*> symbolTable;
static map <string,Type*> strToType = {};
static map <Type*,string> typeToStr = {};

/* def -> specifier -> type */
string defGetTypeName(Node* def){
    Node* type = def->child[0]->child[0];
    return type->get_name();
}

/* def -> Specifier -> StructSpecifier -> STRUCT ID LC DefList RC | STRUCT ID */
string defGetStructName(Node *def){
    Node* stuID = def->child[0]->child[0]->child[1];
    return stuID->get_name();
}

/* VarDec:ID
*    | VarDec LB INT RB
*/
string vardecGetName(Node *vardec){
    while (vardec->child.size()!=1){
        vardec = vardec->child[0];
    }
    return vardec->child[0]->get_name();
}

/* DecList -> Dec
        | Dec COMMA DecList
    Dec -> VarDec
        | VarDec ASSIGN Exp */
Node* decListGetVarDec(Node * decList){
    return decList->child[0]->child[0];
}

FieldList* defPrimitiveType(Node *def,bool flagR){
    Node *decList = def->child[1];
    string dataType = defGetTypeName(def);
    FieldList* head = new FieldList();
    FieldList* ptr = head;

    while(true){
        Node * varDec = decListGetVarDec(decList);
        string varName = vardecGetName(varDec);
        
        if(symbolTable.count(varName)==1){
            //check data type
            semanticErrors(3,def->get_lineNo());
        }
        if(varDec->child.size()==1){
            //vardec -> id
            symbolTable[varName] = new Type(varName, dataType);
            
        }else{
            //array type
            Type* base = new Type(varName,dataType);
            while(varDec->child.size()!=1){
                int arrSize = varDec->child[2]->get_intVal();
                Array* arr = new Array(base,arrSize);
                Type* upper = new Type(varName,arr);
                base = upper;
                varDec = varDec->child[0];
            }
            symbolTable[varName] = base;
        }

        if(flagR){
            //struct need, return fieldList
            FieldList* next = new FieldList(varName,symbolTable[varName],nullptr);
            ptr->next = next;
            ptr = ptr->next;
        }

        if(decList->child.size()==1){break;}
        decList = decList->child[2];
    }
    return head->next;

}
FieldList* defStructType(Node *def,bool flagR){
    Node *decList = def->child[1];
    string structName = defGetStructName(def);

    FieldList* head = new FieldList();
    FieldList* ptr = head;
    
    while(true){
        Node * varDec = decListGetVarDec(decList);
        string varName = vardecGetName(varDec);
        
        if(symbolTable.count(varName)==1){
            //redefine
            semanticErrors(3,def->get_lineNo());
        }
        if(varDec->child.size()==1){
            //vardec -> id
            symbolTable[varName] = symbolTable[structName];
        }else{
            //array type
            Type* base = symbolTable[structName];
            while(varDec->child.size()!=1){
                int arrSize = varDec->child[2]->get_intVal();
                Array* arr = new Array(base,arrSize);
                Type* upper = new Type(varName,arr);
                base = upper;
                varDec = varDec->child[0];
            }
            symbolTable[varName] = base;
        }
        if(flagR){
            //struct need, return fieldList
            FieldList* next = new FieldList(varName,symbolTable[varName],nullptr);
            ptr->next = next;
            ptr = ptr->next;
        }

        if(decList->child.size()==1){break;}
        decList = decList->child[2];
    }
    return head->next;

}
/* Def -> enter 
    local var
*/
FieldList* defVisit(Node *def,bool flagR){
    if(defGetTypeName(def)=="StructSpecifier"){
        //struct
        return defStructType(def,flagR);
    }else{
        return defPrimitiveType(def,flagR);
    }
}



/* ExtDef -> Specifier ExtDecList SEMI   (var declaration)
   ExtDecList -> VarDec
        | VarDec COMMA ExtDecList */
void extDef_SES(Node *def){
    Node *extDec = def->child[1];
    string dataType = defGetTypeName(def);
    while(true){
        Node *varDec = extDec->child[0];
        string varName = vardecGetName(varDec);
        if(symbolTable.count(varName)==1){
            //redefine
            semanticErrors(3,def->get_lineNo());
        }
        if(varDec->child.size()==1){
            symbolTable[varName] = new Type(varName,dataType);
        }else{
            //array type
            Type* base = new Type(varName,dataType);
            while(varDec->child.size()!=1){
                int arrSize = varDec->child[2]->get_intVal();
                Array* arr = new Array(base,arrSize);
                Type* upper = new Type(varName,arr);
                base = upper;
                varDec = varDec->child[0];
            }
            symbolTable[varName] = base;
        }
        if(extDec->child.size()==1){break;}
        extDec = extDec->child[2];
    }

}

/*  whenever meet SSP -> STRUCT ID {DefList}
    enter this 
*/
void structDec(Node *ssp){
    string stuName = ssp->child[1]->get_name();
    FieldList* head = new FieldList();
    FieldList* ptr = head;
    Node* defL = ssp->child[3];

    if(symbolTable.count(stuName)!=0){
        //struct redefine
        semanticErrors(15,ssp->get_lineNo());
    }

    while(true){
        Node* def = defL->child[0];
        //def visit
        FieldList* defStu = defVisit(def,true);
        while(ptr->next!=nullptr){
            ptr = ptr->next;
        }
        ptr->next = defStu;

        if(defL->child.size()==1){break;}
        defL = defL->child[1];
    }

    Type * stu = new Type(stuName,head->next);
    symbolTable[stuName] = stu;

}

void semanticErrors(int typeID, int lineNo)
{
    switch (typeID)
    {
    case 1:
        printf("Error type 1 at Line %d: undefined variable.\n", lineNo);
        break;
    case 2:
        printf("Error type 2 at Line %d: undefined function.\n", lineNo);
        break;
    case 3:
        printf("Error type 3 at Line %d: variable redefined.\n", lineNo);
        break;
    case 4:
        printf("Error type 4 at Line %d: function redefined.\n", lineNo);
        break;
    case 5:
        printf("Error type 5 at Line %d: unmatching types appear at both sides of the assigment operator.\n", lineNo);
        break;
    case 6:
        printf("Error type 6 at Line %d: value can not be assigned.\n", lineNo);
        break;
    case 7:
        printf("Error type 7 at Line %d: unmatching operands.\n", lineNo);
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
        printf("Error type 15 at Line %d: structure type redefined.\n", lineNo);
        break;
    
    default:
        break;
    }
}

