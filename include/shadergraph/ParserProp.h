#pragma once

#include "shadergraph/VarType.h"
#include "shadergraph/Value.h"

#include <string>
#include <vector>

namespace shadergraph
{

struct ParserProp
{
    enum class Type
    {
        Uniform,
        Function,
        Enum,
        Default,
        Region,
        Export,
    };

    virtual Type GetType() const = 0;
};

struct PropUniform : public ParserProp
{
    virtual Type GetType() const override { return Type::Uniform; }

    std::string name;
    std::string display_name;

    VarType type;
};

struct PropFunction : public ParserProp
{
    virtual Type GetType() const override { return Type::Function; }

    std::string name;
    std::string display_name;

    std::vector<VarType> inputs;
    VarType output;
};

struct PropEnum : public ParserProp
{
    virtual Type GetType() const override { return Type::Enum; }

    std::vector<std::string> types;
};

struct PropDefault : public ParserProp
{
    virtual Type GetType() const override { return Type::Default; }

    ValPtr val = nullptr;
};

struct PropRegion : public ParserProp
{
    virtual Type GetType() const override { return Type::Region; }

    float min = 0, max = 0;
};

struct PropExport : public ParserProp
{
    virtual Type GetType() const override { return Type::Export; }

    std::string display_name;
};

}