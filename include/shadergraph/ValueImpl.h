#pragma once

#include "shadergraph/Value.h"
#include "shadergraph/Variant.h"
#include "shadergraph/ParserProp.h"

#include <vector>

namespace shadergraph
{

struct BoolVal : public Value
{
    bool x = false;
};

struct Bool2Val : public Value
{
    bool xy[2] = { false, false };
};

struct Bool3Val : public Value
{
    bool xyz[3] = { false, false, false };
};

struct Bool4Val : public Value
{
    bool xyzw[4] = { false, false, false, false };
};

struct IntVal : public Value
{
    IntVal() {}
    IntVal(int x) : x(x) {}

    int x = 0;
};

struct Int2Val : public Value
{
    Int2Val() {}
    Int2Val(int x, int y) {
        xy[0] = x;
        xy[1] = y;
    }

    int xy[2] = { 0, 0 };
};

struct Int3Val : public Value
{
    Int3Val() {}
    Int3Val(int x, int y, int z) {
        xyz[0] = x;
        xyz[1] = y;
        xyz[2] = z;
    }

    int xyz[3] = { 0, 0, 0 };
};

struct Int4Val : public Value
{
    Int4Val() {}
    Int4Val(int x, int y, int z, int w) {
        xyzw[0] = x;
        xyzw[1] = y;
        xyzw[2] = z;
        xyzw[3] = w;
    }

    int xyzw[4] = { 0, 0, 0, 0 };
};

struct FloatVal : public Value
{
    FloatVal() {}
    FloatVal(float x) : x(x) {}

    float x = 0;
};

struct Float2Val : public Value
{
    Float2Val() {}
    Float2Val(float x, float y) {
        xy[0] = x;
        xy[1] = y;
    }

    float xy[2] = { 0, 0 };
};

struct Float3Val : public Value
{
    Float3Val() {}
    Float3Val(float x, float y, float z) {
        xyz[0] = x;
        xyz[1] = y;
        xyz[2] = z;
    }

    float xyz[3] = { 0, 0, 0 };
};

struct Float4Val : public Value
{
    Float4Val() {}
    Float4Val(float x, float y, float z, float w) {
        xyzw[0] = x;
        xyzw[1] = y;
        xyzw[2] = z;
        xyzw[3] = w;
    }

    float xyzw[4] = { 0, 0, 0, 0 };
};

struct Matrix2Val : public Value
{
    float m[4] = { 1, 0, 0, 1 };
};

struct Matrix3Val : public Value
{
    float m[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
};

struct Matrix4Val : public Value
{
    float m[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
};

struct StringVal : public Value
{
    StringVal() {}
    StringVal(const std::string& str) : str(str) {}

    std::string str;
};

struct ArrayVal : public Value
{
    VarType type = VarType::Invalid;
    std::vector<ValPtr> items;
};

struct StructVal : public Value
{
    std::string type_name;

    std::vector<Variant> children;
};

struct UniformVal : public Value
{
    Variant var;

    std::vector<std::shared_ptr<ParserProp>> desc;
};

struct FunctionVal : public Value
{
    std::string code;

    std::vector<Variant> inputs;
    Variant output;

    std::vector<Variant> declares;

    std::vector<std::shared_ptr<ParserProp>> desc;
};

}