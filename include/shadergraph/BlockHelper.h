#pragma once

#include "shadergraph/Variant.h"

#include <dag/Node.h>

namespace shadergraph
{

class BlockHelper
{
public:
    static VarType ResolveType(const dag::Node<Variant>::Port& p);

    static VarType ResolveBinOpRetType(const dag::Node<Variant>::Port& a,
        const dag::Node<Variant>::Port& b);

    static std::string StringFormat(const std::string str, ...);

}; // BlockHelper

}