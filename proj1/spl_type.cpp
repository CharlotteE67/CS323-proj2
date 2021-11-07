#include "spl_type.hpp"

Array::Array(Type *base, int size) : base(base), size(size) {}

FieldList::FieldList(string name, Type *type, FieldList *next) : name(std::move(name)), type(type), next(next) {}


