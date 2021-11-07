#ifndef SPL_TYPE_HPP
#define SPL_TYPE_HPP

#include <string>
#include <vector>
#include "spl_node.hpp"

using namespace std;

enum class CATEGORY {
    PRIMITIVE, ARRAY, STRUCTURE
};

class Type;

class Array;

class FieldList;

union dTypes {
    enum Node_TYPE nt;
    Array* arr;
    FieldList* fl;
};

class Type {
public:
    string name;
    CATEGORY category = CATEGORY::PRIMITIVE;
    dTypes type;
    Type *returnType = nullptr; // 存储返回值的类型

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
