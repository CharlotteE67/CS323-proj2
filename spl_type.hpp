#ifndef SPL_TYPE_HPP
#define SPL_TYPE_HPP

#include <string>
#include <vector>

using namespace std;

enum class CATEGORY {
    PRIMITIVE, ARRAY, STRUCTURE
};
enum class Primitive {
    INT,
    FLOAT,
    CHAR
};

class Type;

class Array;

class FieldList;

union dTypes {
    Primitive pri;
    Array *arr;
    FieldList *fl;
};

class Type {
public:
    string name;
    CATEGORY category;
    dTypes type;
    Type *typePointer = nullptr; // For func:arg list; For struct number: nearest outlayer struct

    Type();

    Type(string name, string pri);

    Type(string name, Array *arr);

    Type(string name, CATEGORY cat);

    Type* get_typePtr(){return typePointer;}
    void set_typePtr(Type *ptr){typePointer = ptr;}

    void set_fieldList(FieldList *field){type.fl = field;}

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
