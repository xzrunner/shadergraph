#include "shadergraph/BlockHelper.h"
#include "shadergraph/Evaluator.h"

#include <stdarg.h>

namespace shadergraph
{

VarType BlockHelper::ResolveType(const Evaluator& eval,
                                 const dag::Node<Variant>::Port& p)
{
    auto type = eval.QueryRealType(&p.var.type);
    if (type != VarType::Invalid && type != VarType::Dynamic) {
        return type;
    }

    if (p.conns.empty()) {
        return VarType::Invalid;
    }

    assert(p.conns.size() == 1);
    auto node = p.conns[0].node.lock();
    int idx = p.conns[0].idx;
    if (!node) {
        return VarType::Invalid;
    }
    type = node->GetExports()[idx].var.type.type;
    if (type == VarType::Dynamic) {
        return eval.QueryRealType(&node->GetExports()[idx].var.type);
    } else {
        return type;
    }
}

VarType BlockHelper::ResolveBinOpRetType(const Evaluator& eval,
                                         const dag::Node<Variant>::Port& a,
                                         const dag::Node<Variant>::Port& b)
{
    auto type_a = ResolveType(eval, a);
    auto type_b = ResolveType(eval, b);
    if (type_a == VarType::Invalid &&
        type_b == VarType::Invalid) {
        return VarType::Invalid;
    } else if (type_a == VarType::Invalid) {
        return type_b;
    } else if (type_b == VarType::Invalid) {
        return type_a;
    } else {
        return static_cast<int>(type_a) > static_cast<int>(type_b) ? type_a : type_b;
    }
}

std::string BlockHelper::StringFormat(const std::string fmt, ...)
{
    int size = ((int)fmt.size()) * 2 + 50;   // Use a rubric appropriate for your code
    std::string str;
    va_list ap;
    while (1) {     // Maximum two passes on a POSIX system...
        str.resize(size);
        va_start(ap, fmt);
        int n = vsnprintf((char *)str.data(), size, fmt.c_str(), ap);
        va_end(ap);
        if (n > -1 && n < size) {  // Everything worked
            str.resize(n);
            return str;
        }
        if (n > -1)  // Needed size returned
            size = n + 1;   // For null char
        else
            size *= 2;      // Guess at a larger size (OS specific)
    }
    return str;
}

}