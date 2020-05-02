#pragma once

#include "shadergraph/typedef.h"
#include "shadergraph/Variant.h"
#include "shadergraph/ParserUtility.h"

#include <lexer/Tokenizer.h>
#include <lexer/Parser.h>

namespace shadergraph
{

struct ParserProp;

namespace BlockToken
{
	typedef unsigned int Type;

	static const Type Integer       = 1 <<  0; // integer number
	static const Type Decimal       = 1 <<  1; // decimal number
	static const Type String        = 1 <<  2; // string
    static const Type Comment       = 1 <<  3; // line comment starting with ///

	static const Type OParenthesis  = 1 <<  4; // opening parenthesis: (
	static const Type CParenthesis  = 1 <<  5; // closing parenthesis: )
	static const Type OBrace        = 1 <<  6; // opening brace: {
	static const Type CBrace        = 1 <<  7; // closing brace: }
	static const Type OBracket      = 1 <<  8; // opening bracket: [
	static const Type CBracket      = 1 <<  9; // closing bracket: ]

    static const Type Comma         = 1 << 10; // ,
    static const Type Dot           = 1 << 11; // .
    static const Type Pound         = 1 << 12; // #
    static const Type Colon         = 1 << 13; // :
    static const Type Question      = 1 << 14; // ?

    static const Type Add           = 1 << 15; // +
    static const Type Sub           = 1 << 16; // _
    static const Type Mul           = 1 << 17; // *
    static const Type Div           = 1 << 18; // /
    static const Type Equal         = 1 << 19; // =

	static const Type Eof           = 1 << 20; // end of file
	static const Type Eol           = 1 << 21; // end of line
}

enum class BlockKeyword
{
    Struct,
    Uniform,
};

class BlockTokenizer : public lexer::Tokenizer<BlockToken::Type>
{
public:
    BlockTokenizer(const std::string& str);

    void SetSkipEol(bool skip_eol) {
        m_skip_eol = skip_eol;
    }

protected:
    virtual Token EmitToken() override;

    static const std::string& NumberDelim();

private:
    bool m_skip_eol;

}; // BlockTokenizer

class BlockParser : public lexer::Parser<BlockToken::Type>
{
public:
    BlockParser(const std::string& str);

    void Parse();

    auto& GetUniforms() const { return m_uniforms; }
    auto& GetFunctions() const { return m_funcs; }

private:
    void ParseStruct();
    void ParseUniform();

    Variant ParseVariable();

    virtual std::map<BlockToken::Type, std::string> TokenNames() const override;

private:
    struct Struct
    {
        std::string name;
        std::vector<Variant> vars;
    };

private:
    BlockTokenizer m_tokenizer;

    std::vector<Struct>  m_structs;
    std::vector<Variant> m_uniforms;
    std::vector<Variant> m_funcs;

    std::vector<std::shared_ptr<ParserProp>> m_desc;

    typedef BlockTokenizer::Token Token;

}; // BlockParser

}