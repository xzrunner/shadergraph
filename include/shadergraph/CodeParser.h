#pragma once

#include "shadergraph/Variant.h"

#include <cslang/Declaration.h>

namespace shadergraph
{

class CodeParser
{
public:
    static Variant ToVariant(const cslang::ast::DeclarationNodePtr& var);
    static Variant ToVariant(const cslang::ast::FunctionNodePtr& var);

}; // CodeParser

}