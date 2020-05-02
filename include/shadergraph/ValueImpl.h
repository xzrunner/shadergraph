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
    int x = 0;
};

struct Int2Val : public Value
{
    int xy[2] = { 0, 0 };
};

struct Int3Val : public Value
{
    int xyz[3] = { 0, 0, 0 };
};

struct Int4Val : public Value
{
    int xyzw[4] = { 0, 0, 0, 0 };
};

struct FloatVal : public Value
{
    float x = 0;
};

struct Float2Val : public Value
{
    float xy[2] = { 0, 0 };
};

struct Float3Val : public Value
{
    float xyz[3] = { 0, 0, 0 };
};

struct Float4Val : public Value
{
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

struct ArrayVal : public Value
{
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