#include <string>
#include <map>
#include "symbol_table.hpp"
#include "spl_type.hpp"

map<string, Type*> symbolTable;
static map <string,Type*> strToType = {};
static map <Type*,string> typeToStr = {};

/* def -> specifier -> type */
string defGetTypeName(Node* def){
    Node* type = def->child[0]->child[0];
    return type->get_name();
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

void defPrimitiveType(Node *def){
    Node *decList = def->child[1];
    string dataType = defGetTypeName(def);

    while(true){
        Node * varDec = decListGetVarDec(decList);
        string varName = vardecGetName(varDec);
        
        if(symbolTable.count(varName)==1){
            //check data type
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

        if(decList->child.size()==1){break;}
        decList = decList->child[2];
    }

}

/* Def -> enter 
    local var
*/
void defVisit(Node *def){
    if(defGetTypeName(def)=="StructSpecifier"){
        //struct
        
    }else{
        defPrimitiveType(def);
    }
}

/* ExtDef -> Specifier ExtDecList SEMI   (var declaration)
        | Specifier SEMI                 (specifier = type/struct)
        | Specifier FunDec CompSt        (func declaration)
    ExtDecList -> VarDec
        | VarDec COMMA ExtDecList */


/* ExtDef -> Specifier ExtDecList SEMI   (var declaration)
   ExtDecList -> VarDec
        | VarDec COMMA ExtDecList */
void extDef_SES(Node *def){
    

}





