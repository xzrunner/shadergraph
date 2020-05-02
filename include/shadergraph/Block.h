#pragma once

#include "shadergraph/Variant.h"

#include <dag/Node.h>

namespace shadergraph
{

class Block : public dag::Node<Variant>
{
public:
    Block(const std::string& str);

    //std::string GetMainCode() const;

    auto& GetUniforms() const { return m_uniforms; }
    auto& GetFunctions() const { return m_funcs; }
    int GetCurrFuncIdx() const { return m_curr_func; }

private:
    void Parser(const std::string& str);

    void SetupPorts();

    static dag::Node<Variant>::PortVar PortFromVar(const Variant& var);

private:
    std::vector<Variant> m_uniforms;

    std::vector<Variant> m_funcs;
    int m_curr_func = -1;

    RTTR_ENABLE(dag::Node<Variant>)

}; // Block

}