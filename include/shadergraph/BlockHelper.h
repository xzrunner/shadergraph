#pragma once

#include "shadergraph/Variant.h"

#include <dag/Node.h>

namespace shadergraph
{

class BlockHelper
{
public:
    static VarType ResolveBinOpRetType(const dag::Node<Variant>::Port& a,
        const dag::Node<Variant>::Port& b);

}; // BlockHelper

}