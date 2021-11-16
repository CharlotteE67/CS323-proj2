#ifndef SPL_TYPE_HPP
#define SPL_TYPE_HPP

#include <string>
#include <vector>

using namespace std;

enum class CATEGORY {
    PRIMITIVE, ARRAY, STRUCTURE,STRUCTVAR, FUNCTION
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
    Type *st;
};

class Type {
public:
    string name;
    CATEGORY category;
    dTypes type;
    Type *typePointer = nullptr; // For func: return type; For struct number: nearest outlayer struct

    Type();

    Type(string name, string pri);

    Type(string name, Array *arr);

    Type(string name, Type *rt);

    Type(string name, CATEGORY cat);

    void set_structType(Type *t){type.st = t;}
    Type *get_structType(){return type.st;}

    Type* get_typePtr(){return typePointer;}
    void set_typePtr(Type *ptr){typePointer = ptr;}

    void set_fieldList(FieldList *field){type.fl = field;}
    FieldList* get_fieldList(){return type.fl;}

    void set_argsList(FieldList *fieldList){ type.fl = fieldList; }
    FieldList* get_argsList(){return type.fl;}

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
