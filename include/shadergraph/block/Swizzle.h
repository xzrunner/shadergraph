#pragma once

#include "shadergraph/Block.h"
#include "shadergraph/ParserUtility.h"
#include "shadergraph/Evaluator.h"

#include <cpputil/StringHelper.h>
#include <SM_Vector.h>

namespace shadergraph
{
namespace block
{

class Swizzle : public Block
{
public:
    enum class Channel
    {
        Red,
        Green,
        Blue,
        Alpha
    };

public:
    Swizzle()
    {
        SetupPorts({
            { VarType::Dynamic, "_in" },
        }, {
            { VarType::Dynamic, "_out" },
        });
    }

    virtual std::string GetBody(const Evaluator& eval) const override
    {
		const char* COL[4] = { "r", "g", "b", "a" };

        auto type = eval.QueryRealType(&m_imports[0].var.type);
        auto type_str = TypeToString(type);
		switch (type)
		{
        case VarType::Float4:
			return cpputil::StringHelper::Format("%s #_out# = vec4(#_in#.%s, #_in#.%s, #_in#.%s, #_in#.%s);\n",
                type_str.c_str(), COL[(int)m_r], COL[(int)m_g], COL[(int)m_b], COL[(int)m_a]);
		case VarType::Float3:
			return cpputil::StringHelper::Format("%s #_out# = vec3(#_in#.%s, #_in#.%s, #_in#.%s);\n",
                type_str.c_str(), COL[(int)m_r], COL[(int)m_g], COL[(int)m_b]);
		case VarType::Float2:
			return cpputil::StringHelper::Format("%s #_out# = vec2(#_in#.%s, #_in#.%s);\n",
                type_str.c_str(), COL[(int)m_r], COL[(int)m_g]);
		case VarType::Float:
			return cpputil::StringHelper::Format("%s #_out# = #_in#;\n", type_str.c_str());
		default:
            assert(0);
			return "";
		}
    }

    RTTR_ENABLE(Block)

#define PARM_FILEPATH "shadergraph/block/Swizzle.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Swizzle

}
}