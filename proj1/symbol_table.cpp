#include <string>
#include <map>
#include "symbol_table.hpp"
#include "spl_type.hpp"

map<string, string> symbolTable;
static map <string,Type*> strToType = {};
static map <Type*,string> typeToStr = {};
/* def -> specifier -> type */
Node* defGetTypeNode(Node* def){
    return def->child[0]->child[0];
}

/* VarDec:ID
*    | VarDec LB INT RB
*/
string vardecGetID(Node *vardec){
    while (vardec->child.size()!=1){
        vardec = vardec->child[0];
    }
    return vardec->child[0]->get_name();
}


/* DecList -> Dec
        | Dec COMMA DecList
    Dec -> VarDec
        | VarDec ASSIGN Exp */
string declistGetVarName(Node * declist){
    Node * varDec = declist->child[0]->child[0];
    return vardecGetID(varDec);
}

void defPrimitiveType(Node *def){
    Node *decList = def->child[1];
    string typeName = defGetTypeNode(def)->get_name();
    string varName = declistGetVarName(decList);

    while(decList->child.size()!=1){
        if(symbolTable.count(varName)==1){
            //check data type
        }
        if(decList->child[0]->child[0]->child.size()==1){
            //vardec -> id
            symbolTable[varName] = typeName;
        }else{
            //array type
        }
        decList = decList->child[2];
        varName = declistGetVarName(decList);
    }

}

/* Def -> enter 
    local var
*/
void defVisit(Node *def){
    if(defGetTypeNode(def)->child.empty()){
        defPrimitiveType(def);
    }else{
        //struct
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





