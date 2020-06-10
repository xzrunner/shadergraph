#pragma once

#include "shadergraph/Block.h"
#include "shadergraph/typedef.h"
#include "shadergraph/Variant.h"

#include <dag/Graph.h>

namespace shadergraph
{

struct FunctionVal;

namespace block
{

class Input;
class Output;

class SubGraph : public Block
{
public:
    SubGraph() {}

    virtual std::string GetHeader(const Evaluator& eval) const;

    void Setup(const std::shared_ptr<dag::Graph<Variant>>& graph,
        const std::vector<Variant>& inputs, const std::vector<Variant>& outputs);

private:
    std::string GenCode() const;

private:
    std::shared_ptr<dag::Graph<Variant>> m_graph = nullptr;

    std::string m_func_name;

    RTTR_ENABLE(Block)

}; // SubGraph

}
}