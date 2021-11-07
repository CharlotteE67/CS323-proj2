#ifndef SPL_TYPE_HPP
#define SPL_TYPE_HPP

#include <string>
#include <vector>
#include "spl_node.hpp"

using namespace std;

enum class CATEGORY {
    PRIMITIVE, ARRAY, STRUCTURE
};


class Array;

class FieldList;

enum Primitive{INT,FLOAT,CHAR};
union dTypes {
    Primitive pri;
    Array* Array ;
    FieldList* FieldList;
};

class Type {
public:
    string name;
    CATEGORY category = CATEGORY::PRIMITIVE;
    dTypes type;
    Type *returnType = nullptr; // 存储返回值的类型

    Type() = default;
    Type(string name, string pri): name(name) {
        if(pri == "int"){type.pri = Primitive::INT;}
        else if(pri=="float"){type.pri = Primitive::FLOAT;}
        else if(pri == "char"){type.pri = Primitive::CHAR;} 
    }
    Type(string name, Array* arr): name(name), category(CATEGORY::ARRAY){
        type.Array = arr;
    }
    Type(string name, FieldList* fl): name(name), category(CATEGORY::STRUCTURE){
        type.FieldList = fl;
    }

};

class Array {
public:
    Type *base;
    int size;

    Array(Type *base, int size);
};

class FieldList {
public:
    string name;
    Type *type = nullptr;
    FieldList *next = nullptr;

    FieldList() = default;

    FieldList(string name, Type *type, FieldList *next);

};

#endif
