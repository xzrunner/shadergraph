#include "shadergraph/BlockHelper.h"

namespace shadergraph
{

VarType BlockHelper::ResolveBinOpRetType(const dag::Node<Variant>::Port& a,
                                         const dag::Node<Variant>::Port& b)
{
    if (a.conns.empty() || b.conns.empty()) {
        return VarType::Invalid;
    }

    assert(a.conns.size() == 1
        && b.conns.size() == 1);
    auto node_a = a.conns[0].node.lock();
    auto node_b = b.conns[0].node.lock();
    if (!node_a || !node_b) {
        return VarType::Invalid;
    }

    auto type_a = node_a->GetExports()[a.conns[0].idx].var.type.type;
    auto type_b = node_b->GetExports()[b.conns[0].idx].var.type.type;
    return static_cast<int>(type_a) > static_cast<int>(type_b) ? type_a : type_b;
}

}