#include "shadergraph/block/SubGraph.h"
#include "shadergraph/ValueImpl.h"
#include "shadergraph/Evaluator.h"
#include "shadergraph/block/Bool.h"
#include "shadergraph/block/Int.h"
#include "shadergraph/block/Float.h"
#include "shadergraph/block/Float2.h"
#include "shadergraph/block/Float3.h"
#include "shadergraph/block/Float4.h"
#include "shadergraph/block/Input.h"
#include "shadergraph/block/Output.h"

namespace shadergraph
{
namespace block
{

std::string SubGraph::GetHeader(const Evaluator& eval) const
{
    return GenCode();
}

void SubGraph::Setup(const std::shared_ptr<dag::Graph<Variant>>& graph,
                     const std::vector<Variant>& inputs, const std::vector<Variant>& outputs)
{
    m_graph = graph;

    auto vars2ports = [](std::vector<dag::Node<Variant>::Port>& dst, const std::vector<Variant>& src) {
        dst.reserve(src.size());
        for (auto v : src)
        {
            Variant var;
            var.type = v.type;
            var.name = v.name;

            dag::Node<Variant>::Port port;
            port.var.type = var;
            port.var.full_name = v.name;
            dst.push_back(port);
        }
    };

    std::vector<dag::Node<Variant>::Port> imports, exports;
    vars2ports(imports, inputs);
    vars2ports(exports, outputs);
    SetImports(imports);
    SetExports(exports);

    auto f_val = std::make_shared<FunctionVal>();
    f_val->inputs = inputs;
    if (!outputs.empty()) {
        f_val->output = outputs[0];
    }

    Variant var;
    var.type = VarType::Function;
    var.name = GetName();
    var.val = f_val;
    m_funcs.clear();
    m_funcs.push_back({ var, true });

    if (!outputs.empty()) {
        auto& func = outputs.back();
        assert(func.type == VarType::Function);
        m_func_name = func.name;
    }
}

std::string SubGraph::GenCode() const
{
    std::shared_ptr<Block> ret_node = nullptr;
    assert(!m_exports.empty());
    for (auto& node : m_graph->GetAllNodes()) {
        if (node.second->get_type() == rttr::type::get<Output>() &&
            std::static_pointer_cast<Output>(node.second)->GetVarType() != VarType::Function) {
            ret_node = std::static_pointer_cast<Block>(node.second);
            break;
        }
    }

    if (!ret_node) {
        return "";
    }

    Evaluator eval;
    eval.Rebuild(ret_node);

    std::string ret = eval.GenShaderFuncsCode();
    ret += "\n";

    std::string params_str;
    for (int i = 0, n = m_imports.size(); i < n; ++i)
    {
        auto& param = m_imports[i];
        params_str += TypeToString(param.var.type.type);
        params_str += " ";
        params_str += param.var.type.name;
        if (i != n - 1) {
            params_str += ", ";
        }
    }

    ret += cpputil::StringHelper::Format(R"(
%s %s(%s)
{
%s
return ret;
}
)", TypeToString(m_exports[0].var.type.type).c_str(), m_func_name.c_str(), params_str.c_str(), eval.GenShaderMainCode().c_str());
    return ret;


//    return cpputil::StringHelper::Format(R"(
//%s
//
//float f_scene(vec3 _out)
//{
//%s
//return ret;
//}
//	)", eval.GenShaderFuncsCode().c_str(), eval.GenShaderMainCode().c_str());
}

}
}