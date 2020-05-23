#pragma once

#include "shadergraph/Variant.h"

#include <string>

namespace glsl {
    struct astFunction;
    struct astGlobalVariable;
    struct astStruct;
    struct astTU;
}

namespace shadergraph
{

class CodeParser
{
public:
    static std::string Print(glsl::astTU* tu);

    static Variant ToVariant(const glsl::astFunction* var);
    static Variant ToVariant(const glsl::astGlobalVariable* var);
    static Variant ToVariant(const glsl::astStruct* var);

private:
    static ValPtr CreateValue(VarType type);

}; // CodeParser

}