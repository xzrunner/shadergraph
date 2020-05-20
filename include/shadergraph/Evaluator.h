#pragma once

#include "shadergraph/typedef.h"
#include "shadergraph/VarType.h"
#include "shadergraph/Value.h"

#include <dag/Node.h>

#include <vector>
#include <set>
#include <unordered_map>

namespace shadergraph
{

struct Variant;

class Evaluator
{
public:
    struct Uniform
    {
        std::string name;
        VarType     type;
        ValPtr      val = nullptr;
    };

public:
    Evaluator() {}

    void Rebuild(const BlockPtr& block);

    std::string GenShaderCode() const;
    std::vector<Uniform> CalcUniformValues() const;

    VarType QueryRealType(const Variant* var) const;

private:
    std::string GenShaderHeaderCode() const;
    std::string GenShaderUniformsCode() const;
    std::string GenShaderFuncsCode() const;
    std::string GenShaderMainCode() const;

    void Clear();

    void Sort(const std::vector<BlockPtr>& blocks);
    void Resolve();

    void Concatenate();

    void ResolveFunctions();
    void ResolveVariants();

    void Rename(std::string& str, const Block& block) const;

    static void GetAntecedentNodes(const BlockPtr& src, std::vector<BlockPtr>& dst);
    static bool IsFuncNotExport(const Block& block, int func_idx);

    static Variant CalcValue(const dag::Node<Variant>::PortAddr& conn);

    static std::string GetDefaultValueString(VarType type);

private:
    BlockPtr m_block = nullptr;
    std::vector<BlockPtr> m_blocks;

    std::set<std::string> m_symbols;

    std::unordered_map<const Variant*, std::string> m_real_names;
    std::unordered_map<const Variant*, std::string> m_real_funcs;

    std::unordered_map<const Variant*, VarType> m_real_types;

}; // Evaluator

}