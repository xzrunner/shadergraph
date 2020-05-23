#include "shadergraph/CodeParser.h"
#include "shadergraph/ValueImpl.h"
#include "shadergraph/ParserUtility.h"

#include <glsl-parser/parser.h>
#include <cpputil/StringHelper.h>

#include <assert.h>

namespace
{

using namespace glsl;

#undef KEYWORD
#define KEYWORD(X) #X,
const char *kTypes[] = {
    #include <glsl-parser/lexemes.h>
};
#undef KEYWORD
#define KEYWORD(...)

#undef OPERATOR
#define OPERATOR(N, S, P) S,
const char *kOperators[] = {
    #include <glsl-parser/lexemes.h>
};
#undef OPERATOR
#define OPERATOR(...)

//#define print(...)           \
//    do {                     \
//        printf(__VA_ARGS__); \
//    } while (0)

#define print(out, ...)           \
    do {                          \
        out << cpputil::StringHelper::Format(__VA_ARGS__); \
    } while (0)

void printExpression(std::ostream& out, astExpression *expression);
void printStatement(std::ostream& out, astStatement *statement);

void printBuiltin(std::ostream& out, astBuiltin *builtin) {
    print(out, "%s", kTypes[builtin->type]);
}

void printType(std::ostream& out, astType *type) {
    if (type->builtin)
        printBuiltin(out, (astBuiltin*)type);
    else
        print(out, "%s", (out, (astStruct*)type)->name);
}

void printIntConstant(std::ostream& out, astIntConstant *expression) {
    print(out, "%d", expression->value);
}

void printUIntConstant(std::ostream& out, astUIntConstant *expression) {
    print(out, "%du", expression->value);
}

void printFloatConstant(std::ostream& out, astFloatConstant *expression) {
    char format[1024];
    snprintf(format, sizeof format, "%g", expression->value);
    if (!strchr(format, '.'))
        print(out, "%g.0", expression->value);
    else
        print(out, "%s", format);
}

void printDoubleConstant(std::ostream& out, astDoubleConstant *expression) {
    print(out, "%g", expression->value);
}

void printBoolConstant(std::ostream& out, astBoolConstant *expression) {
    print(out, "%s", expression->value ? "true" : "false");
}

void printArraySize(std::ostream& out, const std::vector<astConstantExpression*> &arraySizes) {
    for (size_t i = 0; i < arraySizes.size(); i++) {
        print(out, "[");
        printExpression(out, arraySizes[i]);
        print(out, "]");
    }
}

void printVariable(std::ostream& out, astVariable *variable, bool nameOnly = false) {
    if (variable->isPrecise)
        print(out, "precise ");

    if (nameOnly) {
        print(out, "%s", variable->name);
        return;
    }

    printType(out, variable->baseType);
    print(out, " %s", variable->name);

    if (nameOnly)
        return;

    if (variable->isArray)
        printArraySize(out, variable->arraySizes);
}

void printStorage(std::ostream& out, int storage) {
    switch (storage) {
    case kConst:
        print(out, "const ");
        break;
    case kIn:
        print(out, "in ");
        break;
    case kOut:
        print(out, "out ");
        break;
    case kAttribute:
        print(out, "attribute ");
        break;
    case kUniform:
        print(out, "uniform ");
        break;
    case kVarying:
        print(out, "varying ");
        break;
    case kBuffer:
        print(out, "buffer ");
        break;
    case kShared:
        print(out, "shared ");
        break;
    }
}

void printAuxiliary(std::ostream& out, int auxiliary) {
    switch (auxiliary) {
    case kCentroid:
        print(out, "centroid ");
        break;
    case kSample:
        print(out, "sample ");
        break;
    case kPatch:
        print(out, "patch ");
        break;
    }
}

void printMemory(std::ostream& out, int memory) {
    if (memory & kCoherent) print(out, "coherent ");
    if (memory & kVolatile) print(out, "volatile ");
    if (memory & kRestrict) print(out, "restrict ");
    if (memory & kReadOnly) print(out, "readonly ");
    if (memory & kWriteOnly) print(out, "writeonly ");
}

void printPrecision(std::ostream& out, int precision) {
    switch (precision) {
        case kLowp:
            print(out, "lowp ");
            break;
        case kMediump:
            print(out, "mediump ");
            break;
        case kHighp:
            print(out, "highp ");
            break;
    }
}

void printGlobalVariable(std::ostream& out, astGlobalVariable *variable) {
    std::vector<astLayoutQualifier*> &qualifiers = variable->layoutQualifiers;
    if (variable->layoutQualifiers.size()) {
        print(out, "layout (");
        for (size_t i = 0; i < qualifiers.size(); i++) {
            astLayoutQualifier *qualifier = qualifiers[i];
            print(out, "%s", qualifier->name);
            if (qualifier->initialValue) {
                print(out, " = ");
                printExpression(out, qualifier->initialValue);
            }
            if (i != qualifiers.size() - 1)
                print(out, ", ");
        }
        print(out, ") ");
    }

    printStorage(out, variable->storage);
    printAuxiliary(out, variable->auxiliary);
    printMemory(out, variable->memory);
    printPrecision(out, variable->precision);

    if (variable->isInvariant)
        print(out, "invariant ");

    switch (variable->interpolation) {
    case kSmooth:
        print(out, "smooth ");
        break;
    case kFlat:
        print(out, "flat ");
        break;
    case kNoPerspective:
        print(out, "noperspective ");
        break;
    }

    printVariable(out, (astVariable*)variable);

    if (variable->initialValue) {
        print(out, " = ");
        printExpression(out, variable->initialValue);
    }

    print(out, ";\n");
}

void printVariableIdentifier(std::ostream& out, astVariableIdentifier *expression) {
    printVariable(out, expression->variable, true);
}

void printFieldOrSwizzle(std::ostream& out, astFieldOrSwizzle *expression) {
    printExpression(out, expression->operand);
    print(out, ".%s", expression->name);
}

void printArraySubscript(std::ostream& out, astArraySubscript *expression) {
    printExpression(out, expression->operand);
    print(out, "[");
    printExpression(out, expression->index);
    print(out, "]");
}

void printFunctionCall(std::ostream& out, astFunctionCall *expression) {
    print(out, "%s(", expression->name);
    for (size_t i = 0; i < expression->parameters.size(); i++) {
        printExpression(out, expression->parameters[i]);
        if (i != expression->parameters.size() - 1)
            print(out, ", ");
    }
    print(out, ")");
}

void printConstructorCall(std::ostream& out, astConstructorCall *expression) {
    printType(out, expression->type);
    print(out, "(");
    for (size_t i = 0; i < expression->parameters.size(); i++) {
        printExpression(out, expression->parameters[i]);
        if (i != expression->parameters.size() - 1)
            print(out, ", ");
    }
    print(out, ")");
}

enum { kSemicolon = 1 << 0, kNewLine = 1 << 1, kDefault = kSemicolon | kNewLine };

void printFunctionVariable(std::ostream& out, astFunctionVariable *variable, int flags = kDefault ) {
    if (variable->isConst)
        print(out, "const ");
    printVariable(out, (astVariable*)variable);
    if (variable->initialValue) {
        print(out, " = ");
        printExpression(out, variable->initialValue);
    }
    if (flags & kSemicolon) print(out, ";");
    if (flags & kNewLine) print(out, "\n");
}

void printPostIncrement(std::ostream& out, astPostIncrementExpression *expression) {
    printExpression(out, expression->operand);
    print(out, "++");
}

void printPostDecrement(std::ostream& out, astPostDecrementExpression *expression) {
    printExpression(out, expression->operand);
    print(out, "--");
}

void printUnaryMinus(std::ostream& out, astUnaryMinusExpression *expression) {
    print(out, "-");
    printExpression(out, expression->operand);
}

void printUnaryPlus(std::ostream& out, astUnaryPlusExpression *expression) {
    print(out, "+");
    printExpression(out, expression->operand);
}

void printUnaryBitNot(std::ostream& out, astUnaryBitNotExpression *expression) {
    print(out, "~");
    printExpression(out, expression->operand);
}

void printUnaryLogicalNot(std::ostream& out, astUnaryLogicalNotExpression *expression) {
    print(out, "!");
    printExpression(out, expression->operand);
}

void printPrefixIncrement(std::ostream& out, astPrefixIncrementExpression *expression) {
    print(out, "++");
    printExpression(out, expression->operand);
}

void printPrefixDecrement(std::ostream& out, astPrefixDecrementExpression *expression) {
    print(out, "--");
    printExpression(out, expression->operand);
}

void printAssign(std::ostream& out, astAssignmentExpression *expression) {
    printExpression(out, expression->operand1);
    print(out, " %s ", kOperators[expression->assignment]);
    printExpression(out, expression->operand2);
}

void printSequence(std::ostream& out, astSequenceExpression *expression) {
    print(out, "(");
    printExpression(out, expression->operand1);
    print(out, ", ");
    printExpression(out, expression->operand2);
    print(out, ")");
}

void printOperation(std::ostream& out, astOperationExpression *expression) {
    print(out, "(");
    printExpression(out, expression->operand1);
    print(out, " %s ", kOperators[expression->operation]);
    printExpression(out, expression->operand2);
    print(out, ")");
}

void printTernary(std::ostream& out, astTernaryExpression *expression) {
    print(out, "(");
    printExpression(out, expression->condition);
    print(out, " ? ");
    printExpression(out, expression->onTrue);
    print(out, " : ");
    printExpression(out, expression->onFalse);
    print(out, ")");
}

void printExpression(std::ostream& out, astExpression *expression) {
    switch (expression->type) {
    case astExpression::kIntConstant:
        return printIntConstant(out, (astIntConstant*)expression);
    case astExpression::kUIntConstant:
        return printUIntConstant(out, (astUIntConstant*)expression);
    case astExpression::kFloatConstant:
        return printFloatConstant(out, (astFloatConstant*)expression);
    case astExpression::kDoubleConstant:
        return printDoubleConstant(out, (astDoubleConstant*)expression);
    case astExpression::kBoolConstant:
        return printBoolConstant(out, (astBoolConstant*)expression);
    case astExpression::kVariableIdentifier:
        return printVariableIdentifier(out, (astVariableIdentifier*)expression);
    case astExpression::kFieldOrSwizzle:
        return printFieldOrSwizzle(out, (astFieldOrSwizzle*)expression);
    case astExpression::kArraySubscript:
        return printArraySubscript(out, (astArraySubscript*)expression);
    case astExpression::kFunctionCall:
        return printFunctionCall(out, (astFunctionCall*)expression);
    case astExpression::kConstructorCall:
        return printConstructorCall(out, (astConstructorCall*)expression);
    case astExpression::kPostIncrement:
        return printPostIncrement(out, (astPostIncrementExpression*)expression);
    case astExpression::kPostDecrement:
        return printPostDecrement(out, (astPostDecrementExpression*)expression);
    case astExpression::kUnaryMinus:
        return printUnaryMinus(out, (astUnaryMinusExpression*)expression);
    case astExpression::kUnaryPlus:
        return printUnaryPlus(out, (astUnaryPlusExpression*)expression);
    case astExpression::kBitNot:
        return printUnaryBitNot(out, (astUnaryBitNotExpression*)expression);
    case astExpression::kLogicalNot:
        return printUnaryLogicalNot(out, (astUnaryLogicalNotExpression*)expression);
    case astExpression::kPrefixIncrement:
        return printPrefixIncrement(out, (astPrefixIncrementExpression*)expression);
    case astExpression::kPrefixDecrement:
        return printPrefixDecrement(out, (astPrefixDecrementExpression*)expression);
    case astExpression::kAssign:
        return printAssign(out, (astAssignmentExpression*)expression);
    case astExpression::kSequence:
        return printSequence(out, (astSequenceExpression*)expression);
    case astExpression::kOperation:
        return printOperation(out, (astOperationExpression*)expression);
    case astExpression::kTernary:
        return printTernary(out, (astTernaryExpression*)expression);
    }
}

void printCompoundStatement(std::ostream& out, astCompoundStatement *statement) {
    print(out, " {\n");
    for (size_t i = 0; i < statement->statements.size(); i++)
        printStatement(out, statement->statements[i]);
    print(out, "}\n");
}

void printEmptyStatement(std::ostream& out) {
    print(out, ";");
}

void printDeclarationStatement(std::ostream& out, astDeclarationStatement *statement, int flags = kDefault) {
    for (size_t i = 0; i < statement->variables.size(); i++)
        printFunctionVariable(out, statement->variables[i], flags);
}

void printExpressionStatement(std::ostream& out, astExpressionStatement *statement, int flags = kDefault) {
    printExpression(out, statement->expression);
    if (flags & kSemicolon) print(out, ";");
    if (flags & kNewLine) print(out, "\n");
}

void printIfStetement(std::ostream& out, astIfStatement *statement) {
    print(out, "if(");
    printExpression(out, statement->condition);
    print(out, ")");
    printStatement(out, statement->thenStatement);
    if (statement->elseStatement) {
        print(out, "else");
        if (statement->elseStatement->type == astStatement::kIf)
            print(out, " ");
        printStatement(out, statement->elseStatement);
    }
}

void printSwitchStatement(std::ostream& out, astSwitchStatement *statement) {
    print(out, "switch(");
    printExpression(out, statement->expression);
    print(out, ") {\n");
    for (size_t i = 0; i < statement->statements.size(); i++)
        printStatement(out, statement->statements[i]);
    print(out, "}\n");
}

void printCaseLabelStatement(std::ostream& out, astCaseLabelStatement *statement) {
    if (statement->isDefault)
        print(out, "default");
    else {
        print(out, "case ");
        printExpression(out, statement->condition);
    }
    print(out, ":\n");
}

void printWhileStatement(std::ostream& out, astWhileStatement *statement) {
    print(out, "while(");
    switch (statement->condition->type) {
    case astStatement::kDeclaration:
        printDeclarationStatement(out, (astDeclarationStatement*)statement->condition, false);
        break;
    case astStatement::kExpression:
        printExpressionStatement(out, (astExpressionStatement*)statement->condition, false);
        break;
    }
    print(out, ")");
    printStatement(out, statement->body);
}

void printDoStatement(std::ostream& out, astDoStatement *statement) {
    print(out, "do");
    // deal with non compound (i.e scope) in do loops, e.g: do function_call(); while(expr);
    if (statement->body->type != astStatement::kCompound)
        print(out, " ");
    printStatement(out, statement->body);
    print(out, "while(");
    printExpression(out, statement->condition);
    print(out, ");\n");
}

void printForStatement(std::ostream& out, astForStatement *statement) {
    print(out, "for(");
    if (statement->init) {
        switch (statement->init->type) {
        case astStatement::kDeclaration:
            printDeclarationStatement(out, (astDeclarationStatement*)statement->init, kSemicolon);
            break;
        case astStatement::kExpression:
            printExpressionStatement(out, (astExpressionStatement*)statement->init, kSemicolon);
            break;
        }
    } else {
        print(out, ";");
    }
    if (statement->condition) {
        print(out, " ");
        printExpression(out, statement->condition);
    }
    print(out, ";");
    if (statement->loop) {
        print(out, " ");
        printExpression(out, statement->loop);
    }
    print(out, ")");
    printStatement(out, statement->body);
}

void printContinueStatement(std::ostream& out) {
    print(out, "continue;\n");
}

void printBreakStatement(std::ostream& out) {
    print(out, "break;\n");
}

void printReturnStatement(std::ostream& out, astReturnStatement *statement) {
    if (statement->expression) {
        print(out, "return ");
        printExpression(out, statement->expression);
        print(out, ";\n");
    } else {
        print(out, "return;\n");
    }
}

void printDiscardStatement(std::ostream& out) {
    print(out, "discard;\n");
}

void printStatement(std::ostream& out, astStatement *statement) {
    switch (statement->type) {
    case astStatement::kCompound:
        return printCompoundStatement(out, (astCompoundStatement*)statement);
    case astStatement::kEmpty:
        return printEmptyStatement(out);
    case astStatement::kDeclaration:
        return printDeclarationStatement(out, (astDeclarationStatement*)statement);
    case astStatement::kExpression:
        return printExpressionStatement(out, (astExpressionStatement*)statement);
    case astStatement::kIf:
        return printIfStetement(out, (astIfStatement*)statement);
    case astStatement::kSwitch:
        return printSwitchStatement(out, (astSwitchStatement*)statement);
    case astStatement::kCaseLabel:
        return printCaseLabelStatement(out, (astCaseLabelStatement*)statement);
    case astStatement::kWhile:
        return printWhileStatement(out, (astWhileStatement*)statement);
    case astStatement::kDo:
        return printDoStatement(out, (astDoStatement*)statement);
    case astStatement::kFor:
        return printForStatement(out, (astForStatement*)statement);
    case astStatement::kContinue:
        return printContinueStatement(out);
    case astStatement::kBreak:
        return printBreakStatement(out);
    case astStatement::kReturn:
        return printReturnStatement(out, (astReturnStatement*)statement);
    case astStatement::kDiscard:
        return printDiscardStatement(out);
    }
    print(out, "\n");
}

void printFunctionParameter(std::ostream& out, astFunctionParameter *parameter) {
    printStorage(out, parameter->storage);
    printAuxiliary(out, parameter->auxiliary);
    printMemory(out, parameter->memory);
    printPrecision(out, parameter->precision);
    printType(out, parameter->baseType);
    if (parameter->name)
        print(out, " %s", parameter->name);
    if (parameter->isArray)
        printArraySize(out, parameter->arraySizes);
}

void printFunction(std::ostream& out, const astFunction *function) {
    printType(out, function->returnType);
    print(out, " %s(", function->name);
    for (size_t i = 0, n = function->parameters.size(); i < n; i++) {
        printFunctionParameter(out, function->parameters[i]);
        if (i != n - 1) {
            out << ", ";
        }
    }
    print(out, ")");
    if (function->isPrototype) {
        print(out, ";\n");
        return;
    }
    print(out, " {\n");
    for (size_t i = 0; i < function->statements.size(); i++)
        printStatement(out, function->statements[i]);
    print(out, "}\n");
}

void printStructure(std::ostream& out, astStruct *structure) {
    print(out, "struct ");
    if (structure->name)
        print(out, "%s ", structure->name);
    print(out, "{\n");
    for (size_t i = 0; i < structure->fields.size(); i++) {
        printVariable(out, structure->fields[i]);
        print(out, ";\n");
    }
    print(out, "};\n");
}

void printTU(std::ostream& out, astTU *tu) {
    for (size_t i = 0; i < tu->structures.size(); i++)
        printStructure(out, tu->structures[i]);
    for (size_t i = 0; i < tu->globals.size(); i++)
        printGlobalVariable(out, tu->globals[i]);
    for (size_t i = 0; i < tu->functions.size(); i++)
        printFunction(out, tu->functions[i]);
}

struct sourceFile {
    const char *fileName;
    FILE *file;
    int shaderType;
};

}

namespace shadergraph
{

//CodeParser::CodeParser(const std::string& code)
//{
//    glsl::parser p(code.c_str(), "test");
//    astTU *tu = p.parse(glsl::astTU::kFragment);
//    if (tu) {
//        printTU(tu);
//    } else {
//        fprintf(stderr, "%s\n", p.error());
//    }
//
//    tu->functions;
//}

std::string CodeParser::Print(glsl::astTU* tu)
{
    std::stringstream ss;
    printTU(ss, tu);
    return ss.str();
}

Variant CodeParser::ToVariant(const glsl::astFunction* var)
{
    Variant ret;
    ret.name = var->name;
    ret.type = VarType::Function;

    auto f_var = std::make_shared<FunctionVal>();
    f_var->output.type = TypeAstToVar(var->returnType);
    f_var->output.name = "ret";

    const int num = var->parameters.size();
    f_var->inputs.reserve(num);
    for (auto& param : var->parameters)
    {
        Variant var;
        var.type = TypeAstToVar(param->baseType);
        var.name = param->name;
        f_var->inputs.push_back(var);
    }

    std::stringstream ss;
    printFunction(ss, var);
    f_var->code = ss.str();

    ret.val = f_var;

    return ret;
}

Variant CodeParser::ToVariant(const glsl::astGlobalVariable* var)
{
    assert(var->type == glsl::astVariable::kGlobal);

    Variant ret;
    ret.name = var->name;

    if (var->baseType->builtin)
    {
        auto type = TypeAstToVar(var->baseType);
        ValPtr val = CreateValue(type);
        if (var->storage == glsl::kUniform)
        {
            ret.type = VarType::Uniform;

            auto unif = std::make_shared<UniformVal>();
            unif->var.name = var->name;
            //unif->desc = m_desc;
            unif->var.type = type;
            unif->var.val = val;
            ret.val = unif;
        }
        else
        {
            ret.type = type;
            ret.val = val;
        }
    }

    return ret;
}

Variant CodeParser::ToVariant(const glsl::astStruct* var)
{
    Variant ret;
    ret.name = var->name;
    ret.type = VarType::Struct;

    return ret;
}

ValPtr CodeParser::CreateValue(VarType type)
{
    ValPtr val = nullptr;

    switch (type)
    {
    case VarType::Bool:
        val = std::make_shared<BoolVal>();
        break;
    case VarType::Bool2:
        val = std::make_shared<Bool2Val>();
        break;
    case VarType::Bool3:
        val = std::make_shared<Bool3Val>();
        break;
    case VarType::Bool4:
        val = std::make_shared<Bool4Val>();
        break;

    case VarType::Int:
        val = std::make_shared<IntVal>();
        break;
    case VarType::Int2:
        val = std::make_shared<Int2Val>();
        break;
    case VarType::Int3:
        val = std::make_shared<Int3Val>();
        break;
    case VarType::Int4:
        val = std::make_shared<Int4Val>();
        break;

    case VarType::Float:
        val = std::make_shared<FloatVal>();
        break;
    case VarType::Float2:
        val = std::make_shared<Float2Val>();
        break;
    case VarType::Float3:
        val = std::make_shared<Float3Val>();
        break;
    case VarType::Float4:
        val = std::make_shared<Float4Val>();
        break;

    case VarType::Matrix2:
        val = std::make_shared<Matrix2Val>();
        break;
    case VarType::Matrix3:
        val = std::make_shared<Matrix3Val>();
        break;
    case VarType::Matrix4:
        val = std::make_shared<Matrix4Val>();
        break;
    }

    return val;
}

}