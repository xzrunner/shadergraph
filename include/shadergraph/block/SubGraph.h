#pragma once

#include "shadergraph/Block.h"
#include "shadergraph/typedef.h"
#include "shadergraph/Variant.h"

#include <dag/Graph.h>

namespace shadergraph
{
namespace block
{

class Input;
class Output;

class SubGraph : public Block
{
public:
    SubGraph() {}

    void Setup(const std::vector<Variant>& inputs,
        const std::vector<Variant>& outputs);

    void Build();

private:
    std::string GenCode() const;

    //static Variant BlockToVariant(const Block& block);

private:
    std::vector<std::shared_ptr<Input>> m_inputs;
    std::vector<std::shared_ptr<Output>> m_outputs;

    dag::Graph<Variant> m_graph;

    RTTR_ENABLE(Block)

}; // SubGraph

}
}