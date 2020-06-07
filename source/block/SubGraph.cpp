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

void SubGraph::Setup(const std::vector<Variant>& inputs,
                     const std::vector<Variant>& outputs)
{
    std::vector<shadergraph::Block::Port> imports, exports;
    imports.reserve(inputs.size());
    for (auto i : inputs)
    {
        shadergraph::Variant var;
        var.type = i.type;
        var.name = i.name;

        dag::Node<shadergraph::Variant>::Port port;
        port.var.type = var;
        port.var.full_name = i.name;
        imports.push_back(port);
    }
    exports.reserve(outputs.size());
    for (auto o : outputs)
    {
        shadergraph::Variant var;
        var.type = o.type;
        var.name = o.name;

        dag::Node<shadergraph::Variant>::Port port;
        port.var.type = var;
        port.var.full_name = o.name;
        exports.push_back(port);
    }
    SetImports(imports);
    SetExports(exports);
}

void SubGraph::Build()
{
    auto f_val = std::make_shared<FunctionVal>();

    f_val->code = GenCode();

    for (auto& i : m_inputs)
    {
        shadergraph::Variant var;
        var.type = i->GetType();
        var.name = i->GetName();
        f_val->inputs.push_back(var);
    }
    if (!m_outputs.empty())
    {
        shadergraph::Variant var;
        var.type = m_outputs[0]->GetType();
        f_val->output = var;
    }

    Variant var;
    var.type = VarType::Function;
    var.name = GetName();
    var.val = f_val;
    m_funcs.clear();
    m_funcs.push_back({ var, true });
}

std::string SubGraph::GenCode() const
{
    if (m_outputs.empty()) {
        return "";
    }

    Evaluator eval;
    eval.Rebuild(m_outputs[0]);
    return eval.GenShaderCode();
}

//Variant SubGraph::BlockToVariant(const Block& block)
//{
//    Variant ret;
//    ret.name = block.GetName();
//
//    auto type = block.get_type();
//    if (type == rttr::type::get<block::Bool>()) {
//        ret.type = VarType::Bool;
//    } else if (type == rttr::type::get<block::Int>()) {
//        ret.type = VarType::Int;
//    } else if (type == rttr::type::get<block::Float>()) {
//        ret.type = VarType::Float;
//    } else if (type == rttr::type::get<block::Float2>()) {
//        ret.type = VarType::Float2;
//    } else if (type == rttr::type::get<block::Float3>()) {
//        ret.type = VarType::Float3;
//    } else if (type == rttr::type::get<block::Float4>()) {
//        ret.type = VarType::Float4;
//    } else {
//        assert(0);
//    }
//
//    return ret;
//}

}
}