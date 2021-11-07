#ifndef SPL_TYPE_HPP
#define SPL_TYPE_HPP

#include <string>
#include <vector>
#include "spl_node.hpp"

using namespace std;

enum class CATEGORY {
    PRIMITIVE, ARRAY, STRUCTURE, FUNCTION
};


class Array;

class FieldList;

union dTypes {
    enum Node_TYPE nodeType;
    Array* Array ;
    FieldList* FieldList;
};

class Type {
public:
    string name;
    CATEGORY category = CATEGORY::PRIMITIVE;
    dTypes type;
    Type *returnType = nullptr; // 存储返回值的类型

    static Type *getPrimitiveType(Node_TYPE type);
    static Type *getPrimitiveINT();
    static Type *getPrimitiveFLOAT();
    static Type *getPrimitiveCHAR();

    Type() = default;
    Type(string _name, CATEGORY _category, dTypes _type);

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
