#pragma once

#include "shadergraph/VarType.h"
#include "shadergraph/Value.h"
#include "shadergraph/TypeQualifier.h"

#include <string>

namespace shadergraph
{

struct Variant
{
    Variant() {}
    Variant(VarType t, const std::string& n, 
        const std::shared_ptr<Value>& v = nullptr)
        : type(t), name(n), val(v) {}

    bool operator == (const Variant& v) const {
        return v.type == type && v.name == name 
            && v.qualifier == qualifier && v.val == val;
    }

    VarType     type = VarType::Invalid;
    std::string name;

    TypeQualifier qualifier = TypeQualifier::Null;

    std::shared_ptr<Value> val = nullptr;

}; // Variant

}