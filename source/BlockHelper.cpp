#include "shadergraph/BlockHelper.h"

namespace shadergraph
{

VarType BlockHelper::ResolveType(const dag::Node<Variant>::Port& p)
{
    if (p.conns.empty()) {
        return VarType::Invalid;
    }

    assert(p.conns.size() == 1);
    auto node = p.conns[0].node.lock();
    int idx = p.conns[0].idx;
    return node ? node->GetExports()[idx].var.type.type : VarType::Invalid;
}

VarType BlockHelper::ResolveBinOpRetType(const dag::Node<Variant>::Port& a,
                                         const dag::Node<Variant>::Port& b)
{
    auto type_a = ResolveType(a);
    auto type_b = ResolveType(b);
    if (type_a == VarType::Invalid ||
        type_b == VarType::Invalid) {
        return VarType::Invalid;
    } else {
        return static_cast<int>(type_a) > static_cast<int>(type_b) ? type_a : type_b;
    }
}

}