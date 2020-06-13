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

    assert(!ret_node->GetImports().empty());
    auto& ret_conns = ret_node->GetImports()[0].conns;
    if (ret_conns.empty()) {
        return "";
    }
    assert(ret_conns.size() == 1);
    auto ret_conn = ret_conns[0];
    
    auto ret_type_str = TypeToString(m_exports[0].var.type.type);
    auto main_str = eval.GenShaderMainCode();
    std::string ret_var_name = eval.QueryRealName(&ret_conn.node.lock()->GetExports()[ret_conn.idx].var.type);

    ret += cpputil::StringHelper::Format(R"(
%s %s(%s)
{
%s
return %s;
}
)", ret_type_str.c_str(), m_func_name.c_str(), params_str.c_str(), main_str.c_str(), ret_var_name.c_str());
    return ret;
}

}
}