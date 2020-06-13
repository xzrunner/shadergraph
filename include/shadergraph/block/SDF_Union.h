// from https://iquilezles.org/www/articles/distfunctions/distfunctions.htm

#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class SDF_Union : public Block
{
public:
    SDF_Union() : Block(R"(

float opUnion(float d1, float d2) { return min(d1, d2); }

)") {}

    RTTR_ENABLE(Block)

}; // SDF_Union

}
}