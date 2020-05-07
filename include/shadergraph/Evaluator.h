#pragma once

#include "shadergraph/typedef.h"
#include "shadergraph/VarType.h"
#include "shadergraph/Value.h"

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
    Evaluator(const BlockPtr& block);

    std::string GenShaderCode() const;
    std::vector<Uniform> GetUniformValues() const;

private:
    void Sort(const std::vector<BlockPtr>& blocks);
    void Resolve();

    void Concatenate();

    void ResolveFunctions();
    void ResolveVariants();

    void Rename(std::string& str, const Block& block) const;

    static void GetAntecedentNodes(const BlockPtr& src, std::vector<BlockPtr>& dst);
    static bool IsFuncNotExport(const Block& block, int func_idx);

private:
    std::vector<BlockPtr> m_blocks;

    std::set<std::string> m_symbols;

    std::unordered_map<const Variant*, std::string> m_real_names;

}; // Evaluator

}