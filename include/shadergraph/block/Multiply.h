#pragma once

#include "shadergraph/Block.h"
#include "shadergraph/BlockHelper.h"
#include "shadergraph/ParserUtility.h"

#include <cpputil/StringHelper.h>

namespace shadergraph
{
namespace block
{

class Multiply : public Block
{
public:
    Multiply()
    {
        SetupPorts({
            { VarType::Dynamic, "a", "1" },
            { VarType::Dynamic, "b", "1" }
        }, {
            { VarType::Dynamic, "mul" }
        });
    }
    virtual std::string GetBody() const
    {
        std::string str_sum;
        for (int i = 0, n = m_imports.size(); i < n; ++i)
        {
            str_sum.push_back('#');
            str_sum.push_back(static_cast<char>('a' + i));
            str_sum.push_back('#');
            if (i != n - 1) {
                str_sum += " * ";
            }
        }

        auto type = BlockHelper::ResolveBinOpRetType(m_imports[0], m_imports[1]);
        assert(type != VarType::Invalid);
        return cpputil::StringHelper::Format("%s #mul# = %s;",
            TypeToString(type).c_str(),
            str_sum.c_str()
        );
    }

    RTTR_ENABLE(Block)

}; // Multiply

}
}