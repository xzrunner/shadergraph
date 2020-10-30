#pragma once

#include "shadergraph/VarType.h"

#include <string>

namespace shadergraph
{

struct Value;
struct Variant;

class EvalVar
{
public:
	static std::shared_ptr<Value> ValueTrans(const Variant& var, VarType type);
	static std::string VariantToString(const Variant& var, VarType type);

	static std::string GetDefaultValueString(VarType type);

}; // EvalVar

}