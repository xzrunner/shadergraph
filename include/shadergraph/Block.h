#pragma once

#include "shadergraph/Variant.h"

#include <dag/Node.h>

namespace shadergraph
{

class Block : public dag::Node<Variant>
{
public:
    Block() {}
    Block(const std::string& str);

    virtual std::string GetHeader() const { return ""; }
    virtual std::string GetBody()   const { return ""; }

    auto& GetUniforms() const { return m_uniforms; }
    auto& GetFunctions() const { return m_funcs; }
    int GetCurrFuncIdx() const { return m_curr_func; }

protected:
    void SetupPorts(const std::vector<Variant>& inputs,
        const std::vector<Variant>& outputs);

    void Parser(const std::string& str);

private:
    void SetupPorts();

    static dag::Node<Variant>::PortVar PortFromVar(const Variant& var);

private:
    std::vector<Variant> m_uniforms;

    std::vector<Variant> m_funcs;
    int m_curr_func = -1;

    RTTR_ENABLE(dag::Node<Variant>)

}; // Block

}