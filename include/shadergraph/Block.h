#pragma once

#include "shadergraph/Variant.h"

#include <dag/Node.h>

namespace shadergraph
{

class Evaluator;
struct ParserProp;

class Block : public dag::Node<Variant>
{
public:
    Block() {}
    Block(const std::string& str);

    virtual std::string GetHeader(const Evaluator& eval) const { return ""; }
    virtual std::string GetBody(const Evaluator& eval)   const { return ""; }

    auto& GetVariants() const { return m_global_vars; }
    auto& GetFunctions() const { return m_funcs; }
    int GetCurrFuncIdx() const { return m_curr_func; }

protected:
    void SetupPorts(const std::vector<Variant>& inputs,
        const std::vector<Variant>& outputs);

    void Parser(const std::string& str);

private:
    void SetupCurrFunc(const std::vector<std::shared_ptr<ParserProp>>& props);
    void SetupPorts();

    static dag::Node<Variant>::PortVar PortFromVar(const Variant& var);

private:
    std::vector<std::pair<Variant, bool>> m_funcs;
    int m_curr_func = -1;

    std::vector<Variant> m_global_vars;
    std::vector<Variant> m_structs;

    RTTR_ENABLE(dag::Node<Variant>)

}; // Block

}