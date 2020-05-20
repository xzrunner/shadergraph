#pragma once

#include "shadergraph/Block.h"
#include "shadergraph/BlockHelper.h"
#include "shadergraph/ParserUtility.h"

#include <cpputil/StringHelper.h>

namespace shadergraph
{
namespace block
{

class Subtract : public Block
{
public:
    Subtract()
    {
        SetupPorts({
            { VarType::Dynamic, "a", "0" },
            { VarType::Dynamic, "b", "0" }
        }, {
            { VarType::Dynamic, "sub" }
        });
    }
    virtual std::string GetBody(const Evaluator& eval) const override
    {
        std::string ret;
        for (int i = 0, n = m_imports.size(); i < n; ++i)
        {
            ret.push_back('#');
            ret.push_back(static_cast<char>('a' + i));
            ret.push_back('#');
            if (i != n - 1) {
                ret += " - ";
            }
        }

        auto type = BlockHelper::ResolveBinOpRetType(m_imports[0], m_imports[1]);
        assert(type != VarType::Invalid);
        return cpputil::StringHelper::Format("%s #sub# = %s;",
            TypeToString(type).c_str(),
            ret.c_str()
        );
    }

    RTTR_ENABLE(Block)

}; // Subtract

}
}