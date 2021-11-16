#include "spl_type.hpp"

using namespace std;

Type::Type() {
    typePointer = nullptr;
}

Type::Type(string name, string pri): name(name), category(CATEGORY::PRIMITIVE) {
    if(pri == "int"){type.pri = Primitive::INT;}
    else if(pri=="float"){type.pri = Primitive::FLOAT;}
    else if(pri == "char"){type.pri = Primitive::CHAR;}
}
Type::Type(string name, Array* arr): name(name), category(CATEGORY::ARRAY){
    type.arr = arr;
}

Type::Type(string name, Type *rt): name(name), category(CATEGORY::FUNCTION) {
    typePointer = rt;
}

Type::Type(string name, CATEGORY ca): name(name), category(ca){
    
}

Array::Array(Type *base, int size) : base(base), size(size) {}

FieldList::FieldList() {type = nullptr; next = nullptr;}

FieldList::FieldList(string name, Type *type, FieldList *next) : name(std::move(name)), type(type), next(next) {}

