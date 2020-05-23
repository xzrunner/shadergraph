#pragma once

#include "shadergraph/VarType.h"
#include "shadergraph/ParserProp.h"

#include <lexer/Tokenizer.h>
#include <lexer/Parser.h>

namespace shadergraph
{

struct ParserDesc;

namespace CommentToken
{
    typedef unsigned int Type;

	static const Type Integer       = 1 <<  0; // integer number
	static const Type Decimal       = 1 <<  1; // decimal number
	static const Type String        = 1 <<  2; // string
    static const Type Comment       = 1 <<  3; // line comment starting with //
    static const Type Description   = 1 <<  4; // line description starting with ///

	static const Type OParenthesis  = 1 <<  5; // opening parenthesis: (
	static const Type CParenthesis  = 1 <<  6; // closing parenthesis: )
	static const Type OBracket      = 1 <<  7; // opening bracket: [
	static const Type CBracket      = 1 <<  8; // closing bracket: ]

    static const Type At            = 1 <<  9; // @
    static const Type Ret           = 1 << 10; // ->
    static const Type Equal         = 1 << 11; // =

    static const Type Comma         = 1 << 12; // ,
    static const Type Ellipsis      = 1 << 13; // ...

	static const Type Eof           = 1 << 14; // end of file
	static const Type Eol           = 1 << 15; // end of line
}

class CommentTokenizer : public lexer::Tokenizer<CommentToken::Type>
{
public:
    CommentTokenizer(const std::string& str);

    void SkipLine()
    {
        DiscardUntil("\n\r");
        while (IsWhitespace(CurChar())) {
            Advance();
        }
    }

protected:
    virtual Token EmitToken() override;

    static const std::string& NumberDelim();

private:
    bool m_skip_eol = false;

}; // CommentTokenizer

class CommentParser : public lexer::Parser<CommentToken::Type>
{
public:
    CommentParser(const std::string& str);

    void Parse(std::vector<std::shared_ptr<ParserProp>>& props);

private:
    virtual std::map<CommentToken::Type, std::string>
        TokenNames() const override;

    void ParserDescription(std::vector<std::shared_ptr<ParserProp>>& props);

    std::shared_ptr<Value> ParserValue();

private:
    CommentTokenizer m_tokenizer;

    typedef CommentTokenizer::Token Token;

}; // CommentParser

}