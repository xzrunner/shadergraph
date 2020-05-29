#pragma once

#include "shadergraph/Block.h"

namespace shadergraph
{
namespace block
{

class RoundedPolygon : public Block
{
public:
    RoundedPolygon() : Block(R"(

/////////////////////////////////////////////////
/// <rounded_polygon> @export
/////////////////////////////////////////////////

// proposed solution from
// http://stackoverflow.com/questions/26070410/robust-atany-x-on-glsl-for-converting-xy-coordinate-to-angle
// swaps params when |x| <= |y|
float atan2(float y, float x)
{
    bool s = (abs(x) > abs(y));
    return mix(3.14159265358979/2.0 - atan(x,y), atan(y,x), s);
}

// https://stackoverflow.com/questions/7610631/glsl-mod-vs-hlsl-fmod
float fmod(float x, float y)
{
    //return x ¨C y * trunc(x/y);
    return x - y * floor(x/y);
}

// https://docs.unity3d.com/Packages/com.unity.shadergraph@9.0/manual/Rounded-Polygon-Node.html
float rounded_polygon(vec2 uv, float width, float height, float sides, float roundness)
{
    const float PI = 3.14159265359;
    const float HALF_PI = 1.570796327;

    uv = uv * 2. + vec2(-1.,-1.);
    // fixme: parser
    //float epsilon = 1e-6;
    //uv.x = uv.x / ( width + (width==0?1:0)*epsilon);
    //uv.y = uv.y / ( height + (height==0?1:0)*epsilon);
    //roundness = clamp(roundness, 1e-6, 1.);
    float epsilon = 0;
    uv.x = uv.x / ( width + ((width==0)?1:0)*epsilon);
    uv.y = uv.y / ( height + ((height==0)?1:0)*epsilon);
    roundness = clamp(roundness, 0., 1.);
    float i_sides = floor( abs( sides ) );
    float fullAngle = 2. * PI / i_sides;
    float halfAngle = fullAngle / 2.;
    float opositeAngle = HALF_PI - halfAngle;
    float diagonal = 1. / cos( halfAngle );
    // Chamfer values
    float chamferAngle = roundness * halfAngle; // Angle taken by the chamfer
    float remainingAngle = halfAngle - chamferAngle; // Angle that remains
    float ratio = tan(remainingAngle) / tan(halfAngle); // This is the ratio between the length of the polygon's triangle and the distance of the chamfer center to the polygon center
    // Center of the chamfer arc
    vec2 chamferCenter = vec2(
        cos(halfAngle) ,
        sin(halfAngle)
    )* ratio * diagonal;
    // starting of the chamfer arc
    vec2 chamferOrigin = vec2(
        1.,
        tan(remainingAngle)
    );
    // Using Al Kashi algebra, we determine:
    // The distance distance of the center of the chamfer to the center of the polygon (side A)
    float distA = length(chamferCenter);
    // The radius of the chamfer (side B)
    float distB = 1. - chamferCenter.x;
    // The refence length of side C, which is the distance to the chamfer start
    float distCref = length(chamferOrigin);
    // This will rescale the chamfered polygon to fit the uv space
    // diagonal = length(chamferCenter) + distB;
    float uvScale = diagonal;
    uv *= uvScale;
    vec2 polaruv = vec2 (
        atan2( uv.y, uv.x ),
        length(uv)
    );
    polaruv.x += HALF_PI + 2*PI;
    polaruv.x = fmod( polaruv.x + halfAngle, fullAngle );
    polaruv.x = abs(polaruv.x - halfAngle);
    uv = vec2( cos(polaruv.x), sin(polaruv.x) ) * polaruv.y;
    // Calculate the angle needed for the Al Kashi algebra
    float angleRatio = 1. - (polaruv.x-remainingAngle) / chamferAngle;
    // Calculate the distance of the polygon center to the chamfer extremity
    float distC = sqrt( distA*distA + distB*distB - 2.*distA*distB*cos( PI - halfAngle * angleRatio ) );
    float ret = uv.x;
    // fixme: parser
    //float chamferZone = ( halfAngle - polaruv.x ) < chamferAngle ? 1 : 0;
    float chamferZone = (( halfAngle - polaruv.x ) < chamferAngle) ? 1 : 0;
    ret = mix( uv.x, polaruv.y / distC, chamferZone );
    // Output this to have the shape mask instead of the distance field
    ret = clamp((1 - ret) / fwidth(ret), 0.0, 1.0);
    return ret;
}

)") {}

    RTTR_ENABLE(Block)

}; // RoundedPolygon

}
}