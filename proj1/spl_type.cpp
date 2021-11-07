#include "spl_type.hpp"

Type::Type(string _name, CATEGORY _category, dTypes _type):
        name(std::move(_name)),
        category(_category),
        type(std::move(_type)) {
}

Array::Array(Type *base, int size) : base(base), size(size) {}

FieldList::FieldList(string name, Type *type, FieldList *next) : name(std::move(name)), type(type), next(next) {}


