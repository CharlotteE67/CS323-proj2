#ifndef SPL_TYPE_HPP
#define SPL_TYPE_HPP

#include <string>
#include <vector>

using namespace std;

enum class CATEGORY {
    PRIMITIVE, ARRAY, STRUCTURE
};
enum class Primitive{INT,FLOAT,CHAR};

class Type;

class Array;

class FieldList;

union dTypes {
    Primitive pri;
    Array* arr;
    FieldList* fl;
};

class Type {
public:
    string name;
    CATEGORY category;
    dTypes type;
    Type *returnType; // 存储返回值的类型

    Type();

    Type(string name, string pri);

    Type(string name, Array* arr);

    Type(string name, FieldList* fl);

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
    Type *type;
    FieldList *next;

    FieldList();

    FieldList(string name, Type *type, FieldList *next);

};


#endif
