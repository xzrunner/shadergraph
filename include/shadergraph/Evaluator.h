#pragma once

#include "shadergraph/typedef.h"

#include <vector>

namespace shadergraph
{

class Evaluator
{
public:
    Evaluator(const std::vector<BlockPtr>& blocks);

    std::string GenShaderCode() const;

private:
    void Sort(const std::vector<BlockPtr>& blocks);
    void Resolve();

    void ResolveFunctions();

    static bool IsFuncNotExport(const Block& block, int func_idx);

private:
    std::vector<BlockPtr> m_blocks;

}; // Evaluator

}