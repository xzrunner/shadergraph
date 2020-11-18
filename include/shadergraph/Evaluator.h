#pragma once

#include "shadergraph/typedef.h"
#include "shadergraph/VarType.h"
#include "shadergraph/Value.h"
#include "shadergraph/Variant.h"

#include <dag/Node.h>

#include <vector>
#include <set>
#include <unordered_map>

namespace shadergraph
{

class Evaluator
{
public:
    struct Uniform
    {
        std::string name;
        VarType     type;
        ValPtr      val = nullptr;
    };

    enum class ShaderType
    {
        Vert,
        Frag,
    };

public:
    Evaluator() {}

    void Rebuild(const BlockPtr& block);

    bool HasBlock(const BlockPtr& block) const;
    void AddBlock(const BlockPtr& block);

    std::string GenShaderCode(ShaderType shader_type) const;
    std::vector<Uniform> CalcUniformValues() const;

    VarType QueryRealType(const Variant* var) const;
    std::string QueryRealName(const Variant* var) const;

    std::string GenShaderHeaderCode(ShaderType shader_type) const;
    std::string GenShaderGlobalVarsCode(bool only_uniform) const;
    std::string GenShaderFuncsCode() const;
    std::string GenShaderMainCode(ShaderType shader_type) const;

    bool IsEmpty() const { return m_blocks.empty(); }

private:
    void Clear();

    void Sort(const std::vector<BlockPtr>& blocks);
    void Resolve();

    void Concatenate();

    void ResolveFunctions();
    void ResolveVariants();

    void Rename(std::string& str, const Block& block) const;

    std::string ResolveFuncCall(const Block& block, size_t out_idx) const;

    static void GetAntecedentNodes(const BlockPtr& src, std::vector<BlockPtr>& dst);

    static Variant CalcValue(const dag::Node<Variant>::PortAddr& conn);

private:
    struct VarNames
    {
        void Init(const std::unordered_map<const Variant*, std::string>& vars)
        {
            var2name = vars;

            used.clear();
            for (auto& itr : vars) {
                used.insert(itr.second);
            }
        }

        void Clear()
        {
            used.clear();
            var2name.clear();
        }

        void Insert(const Variant* var, const std::string& name) {
            used.insert(name);
            var2name.insert({ var, name });
        }

        std::set<std::string> used;
        std::unordered_map<const Variant*, std::string> var2name;
    };

private:
    BlockPtr m_block = nullptr;
    std::vector<BlockPtr> m_blocks;

    VarNames m_real_names;
    std::unordered_map<const Variant*, std::string> m_real_funcs;

    std::unordered_map<const Variant*, VarType> m_real_types;

}; // Evaluator

}