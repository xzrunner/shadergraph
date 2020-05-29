#pragma once

#include "shadergraph/Variant.h"

#include <cslang/Declaration.h>

namespace shadergraph
{

class CommentParser;

class CodeParser
{
public:
    static Variant ToVariant(const CommentParser& desc,
        const cslang::ast::DeclarationNodePtr& src);
    static Variant ToVariant(const CommentParser& desc,
        const cslang::ast::FunctionNodePtr& src);

}; // CodeParser

}