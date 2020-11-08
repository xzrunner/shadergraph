#pragma once

#include "shadergraph/Block.h"

#include <cpputil/StringHelper.h>

namespace shadergraph
{
namespace block
{

class SampleTexNoTile : public Block
{
public:
    enum class ImplType
    {
        Impl0,
        Impl1,
    };

public:
    SampleTexNoTile()
    {
        SetupPorts({
            { VarType::Sampler2D, "tex" },
            { VarType::Float2,    "uv" },
        }, {
            { VarType::Float4, "rgba" },
        });
    }

    virtual std::string GetHeader(const Evaluator& eval) const override
    {
        return cpputil::StringHelper::Format(R"(
vec4 hash4( vec2 p ) { return fract(sin(vec4( 1.0+dot(p,vec2(37.0,17.0)),
                                              2.0+dot(p,vec2(11.0,47.0)),
                                              3.0+dot(p,vec2(41.0,29.0)),
                                              4.0+dot(p,vec2(23.0,31.0))))*103.0); }

vec4 textureNoTile1( sampler2D samp, in vec2 uv )
{
    ivec2 iuv = ivec2( floor( uv ) );
     vec2 fuv = fract( uv );

    // generate per-tile transform
    vec4 ofa = hash4( iuv + ivec2(0,0) );
    vec4 ofb = hash4( iuv + ivec2(1,0) );
    vec4 ofc = hash4( iuv + ivec2(0,1) );
    vec4 ofd = hash4( iuv + ivec2(1,1) );

    vec2 ddx = dFdx( uv );
    vec2 ddy = dFdy( uv );

    // transform per-tile uvs
    ofa.zw = sign( ofa.zw-0.5 );
    ofb.zw = sign( ofb.zw-0.5 );
    ofc.zw = sign( ofc.zw-0.5 );
    ofd.zw = sign( ofd.zw-0.5 );

    // uv's, and derivatives (for correct mipmapping)
    vec2 uva = uv*ofa.zw + ofa.xy, ddxa = ddx*ofa.zw, ddya = ddy*ofa.zw;
    vec2 uvb = uv*ofb.zw + ofb.xy, ddxb = ddx*ofb.zw, ddyb = ddy*ofb.zw;
    vec2 uvc = uv*ofc.zw + ofc.xy, ddxc = ddx*ofc.zw, ddyc = ddy*ofc.zw;
    vec2 uvd = uv*ofd.zw + ofd.xy, ddxd = ddx*ofd.zw, ddyd = ddy*ofd.zw;

    // fetch and blend
    vec2 b = smoothstep( 0.25,0.75, fuv );

    return mix( mix( textureGrad( samp, uva, ddxa, ddya ),
                     textureGrad( samp, uvb, ddxb, ddyb ), b.x ),
                mix( textureGrad( samp, uvc, ddxc, ddyc ),
                     textureGrad( samp, uvd, ddxd, ddyd ), b.x), b.y );
}

vec4 textureNoTile2( sampler2D samp, in vec2 uv )
{
    vec2 p = floor( uv );
    vec2 f = fract( uv );

    // derivatives (for correct mipmapping)
    vec2 ddx = dFdx( uv );
    vec2 ddy = dFdy( uv );

    // voronoi contribution
    vec4 va = vec4( 0.0 );
    float wt = 0.0;
    for( int j=-1; j<=1; j++ )
    for( int i=-1; i<=1; i++ )
    {
        vec2 g = vec2( float(i), float(j) );
        vec4 o = hash4( p + g );
        vec2 r = g - f + o.xy;
        float d = dot(r,r);
        float w = exp(-5.0*d );
        vec4 c = textureGrad( samp, uv + o.zw, ddx, ddy );
        va += w*c;
        wt += w;
    }

    // normalization
    return va/wt;
}

//vec4 textureNoTile3( sampler2D samp, in vec2 uv )
//{
//    // sample variation pattern
//    float k = texture( iChannel1, 0.005*x ).x; // cheap (cache friendly) lookup
//
//    // compute index
//    float index = k*8.0;
//    float i = floor( index );
//    float f = fract( index );
//
//    // offsets for the different virtual patterns
//    vec2 offa = sin(vec2(3.0,7.0)*(i+0.0)); // can replace with any other hash
//    vec2 offb = sin(vec2(3.0,7.0)*(i+1.0)); // can replace with any other hash
//
//    // compute derivatives for mip-mapping
//    vec2 dx = dFdx(x), dy = dFdy(x);
//
//    // sample the two closest virtual patterns
//    vec3 cola = textureGrad( iChannel0, x + offa, dx, dy ).xxx;
//    vec3 colb = textureGrad( iChannel0, x + offb, dx, dy ).xxx;
//
//    float sum( vec3 v ) { return v.x+v.y+v.z; }
//
//    // interpolate between the two virtual patterns
//    return mix( cola, colb, smoothstep(0.2,0.8,f-0.1*sum(cola-colb)) );
//}

)");
    }

    virtual std::string GetBody(const Evaluator& eval) const override
    {
        std::string func;
        switch (m_impl)
        {
        case ImplType::Impl0:
            func = "textureNoTile1";
            break;
        case ImplType::Impl1:
            func = "textureNoTile2";
            break;
        default:
            assert(0);
        }

        return "vec4 #rgba# = " + func + "(#tex#, #uv#);\n";
    }

    RTTR_ENABLE(Block)

#define PARM_FILEPATH "shadergraph/block/SampleTexNoTile.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // SampleTexNoTile

}
}