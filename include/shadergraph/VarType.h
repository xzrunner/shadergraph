#pragma once

namespace shadergraph
{

enum class VarType
{
    Invalid,

    Dynamic,

    Void,

    Bool,
    Bool2,
    Bool3,
    Bool4,

    UInt,

    Int,
    Int2,
    Int3,
    Int4,

    Float,
    Float2,
    Float3,
    Float4,

    Matrix2,
    Matrix3,
    Matrix4,

    Sampler2D,
    SamplerCube,

    Array,
    Struct,
    Uniform,
    Function,
};

}