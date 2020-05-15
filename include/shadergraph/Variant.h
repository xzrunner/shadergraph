#pragma once

#include "shadergraph/VarType.h"
#include "shadergraph/Value.h"

#include <string>

namespace shadergraph
{

struct Variant
{
    VarType     type = VarType::Invalid;
    std::string name;
    std::string default_name;

    bool is_const = false;

    std::shared_ptr<Value> val = nullptr;

}; // Variant

}