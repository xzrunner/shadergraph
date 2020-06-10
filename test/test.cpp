#define CATCH_CONFIG_MAIN

#include <catch/catch.hpp>

#include <shadergraph/block/fBm.h>
#include <shadergraph/block/Worley.h>
#include <shadergraph/Evaluator.h>
#include <dag/Node.h>

TEST_CASE("test")
{
    auto noise = std::make_shared<shadergraph::block::Worley>();
    auto frctal = std::make_shared<shadergraph::block::fBm>();
    dag::make_connecting<shadergraph::Variant>({ noise, 1 }, { frctal, 1 });

    shadergraph::Evaluator eval({ frctal, noise });
    auto code = eval.GenShaderCode();

    printf("%s\n", code.c_str());
}