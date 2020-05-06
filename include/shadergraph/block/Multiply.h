#pragma once

#include "shadergraph/Block.h"

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
            { VarType::Dynamic, "a" },
            { VarType::Dynamic, "b" }
        }, {
            { VarType::Dynamic, "out" }
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

        return cpputil::StringHelper::Format("vec2 #out# = %s;", str_sum.c_str());
    }

    RTTR_ENABLE(Block)

}; // Multiply

}
}