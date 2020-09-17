#pragma once

#include "shadergraph/VarType.h"
#include "shadergraph/Variant.h"

#include <string>

namespace shadergraph
{

VarType StringToType(const std::string& token);
std::string TypeToString(VarType type);
std::string TypeToString(const Variant& var);

}