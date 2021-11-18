#ifndef SPL_NODE
#define SPL_NODE

#include <string>
#include <iostream>
#include <vector>
#include "spl_type.hpp"

using namespace std;

enum class Node_TYPE{
    INT,
    FLOAT,
    CHAR,
    NAME,   //IF ELSE ASSIGN etc
    ID,     //ALL IDENTIFIERS
    DTYPE,  //DATATYPE, INCLUDING INT,FLOAT & CHAR
    LINE
};

class Node{
private:
    string name;
    //string id_name;// == string_value
    Node_TYPE TYPE;
    Type *varType;
    union{
        int lineno;
        int int_value;
        float float_value;
        char char_value;
    };
    


public:
    vector <Node*> child;
    bool assignable = false;
    Node();

    void print(int depth);

    Node(int val);
    Node(float val);
    Node(char val);
    
    Node(string name);
    Node(string name,Node_TYPE type);
    Node(string name,int line_no);
    Node(string name,int line_no, vector<Node*>& child);
    Node(string name,int line_no,Node_TYPE type);

    void set_child(vector<Node*>&);

    void show(int depth);

    string get_name(){return name;}
    Node_TYPE get_type(){return TYPE;}
    int get_intVal(){return int_value;}
    int get_lineNo(){return lineno;}
    Type* get_varType(){return varType;}
    void set_varType(Type *var){varType = var;}
    void set_assignable(bool b) { assignable = b; }

};


#endif