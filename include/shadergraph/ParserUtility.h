#pragma once

#include "shadergraph/VarType.h"

#include <string>

namespace glsl { struct astType; }

namespace shadergraph
{

VarType StringToType(const std::string& token);
std::string TypeToString(VarType type);

VarType TypeAstToVar(const glsl::astType* type);

}