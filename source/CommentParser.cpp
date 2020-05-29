#include "shadergraph/CommentParser.h"
#include "shadergraph/ParserUtility.h"
#include "shadergraph/ValueImpl.h"

#include <lexer/Exception.h>

namespace shadergraph
{

//////////////////////////////////////////////////////////////////////////
// class CommentTokenizer
//////////////////////////////////////////////////////////////////////////

CommentTokenizer::CommentTokenizer(const std::string& str)
    : lexer::Tokenizer<CommentToken::Type>(str.c_str(), str.c_str() + str.length(), "\"", '\\')
{
}

lexer::Tokenizer<CommentToken::Type>::Token
CommentTokenizer::EmitToken()
{
    while (!Eof())
	{
        size_t start_line   = Line();
        size_t start_column = Column();
        const char* c = CurPos();
        switch (*c)
		{
            case '/':
                Advance();
                if (CurChar() == '/')
				{
                    Advance();
                    if (CurChar() == '/')
                    {
                        Advance();
                        if (CurChar() == '/') {
                            const char* e = DiscardUntil("\n\r");
                            return Token(CommentToken::Comment, c, e, Offset(c), start_line, start_column);
                        } else {
                            return Token(CommentToken::Description, c, c + 3, Offset(c), start_line, start_column);
                        }
                    }
                    else
                    {
                        const char* e = DiscardUntil("\n\r");
                        return Token(CommentToken::Comment, c, e, Offset(c), start_line, start_column);
                    }
                }
                else
                {
                    Advance();
                    return Token(0, c, c + 1, Offset(c), start_line, start_column);
                }
                break;
            case '(':
                Advance();
                return Token(CommentToken::OParenthesis, c, c+1, Offset(c), start_line, start_column);
            case ')':
                Advance();
                return Token(CommentToken::CParenthesis, c, c+1, Offset(c), start_line, start_column);
            case '[':
                Advance();
                return Token(CommentToken::OBracket, c, c+1, Offset(c), start_line, start_column);
            case ']':
                Advance();
                return Token(CommentToken::CBracket, c, c+1, Offset(c), start_line, start_column);
            case '<':
                Advance();
                return Token(CommentToken::OAngle, c, c + 1, Offset(c), start_line, start_column);
            case '>':
                Advance();
                return Token(CommentToken::CAngle, c, c + 1, Offset(c), start_line, start_column);
            case '@':
                Advance();
                return Token(CommentToken::At, c, c + 1, Offset(c), start_line, start_column);
            case '-':
                Advance();
                if (CurChar() == '>')
                {
                    Advance();
                    return Token(CommentToken::Ret, c, c + 2, Offset(c), start_line, start_column);
                }
                else
                {
                    return Token(0, c, c + 1, Offset(c), start_line, start_column);
                }
            case '=':
                Advance();
                return Token(CommentToken::Equal, c, c + 1, Offset(c), start_line, start_column);
            case ',':
                Advance();
                return Token(CommentToken::Comma, c, c + 1, Offset(c), start_line, start_column);
            case '\n':
                if (!m_skip_eol) {
                    Advance();
                    return Token(CommentToken::Eol, c, c+1, Offset(c), start_line, start_column);
                }
//                SwitchFallthrough();
            case '\r':
            case ' ':
            case '\t':
            case ';':
            {
                static const std::string whitespace(Whitespace() + ";");
                DiscardWhile(whitespace);
            }
                break;
            case '.':
                Advance();
                if (CurChar() == '.')
                {
                    Advance();
                    if (CurChar() == '.')
                    {
                        Advance();
                        return Token(CommentToken::Ellipsis, c, c + 3, Offset(c), start_line, start_column);
                    }
                }
                return Token(0, c, c + 1, Offset(c), start_line, start_column);
            default: { // whitespace, integer, decimal or word
                const char* e = ReadInteger(NumberDelim());
				if (e != nullptr) {
					return Token(CommentToken::Integer, c, e, Offset(c), start_line, start_column);
				}

                e = ReadDecimal(NumberDelim());
				if (e != nullptr) {
					return Token(CommentToken::Decimal, c, e, Offset(c), start_line, start_column);
				}

                static const std::string str_separator(Whitespace() + ";{}[]()<>,.#:?+-*/=");
                e = ReadUntil(str_separator);
				if (e == nullptr) {
					throw lexer::ParserException(start_line, start_column, "Unexpected character: " + std::string(c, 1));
				}

                return Token(CommentToken::String, c, e, Offset(c), start_line, start_column);
            }
        }
    }
    return Token(CommentToken::Eof, nullptr, nullptr, Length(), Line(), Column());
}

const std::string& CommentTokenizer::NumberDelim()
{
    static const std::string number_delim(Whitespace() + ",)]}>");
    return number_delim;
}

//////////////////////////////////////////////////////////////////////////
// class CommentParser
//////////////////////////////////////////////////////////////////////////

CommentParser::CommentParser(const std::string& str)
    : m_tokenizer(CommentTokenizer(str))
{
}

void CommentParser::Parse()
{
    Token token = m_tokenizer.PeekToken();
    CommentToken::Type token_type = token.GetType();
    while (token_type != CommentToken::Eof)
    {
        while (token_type != CommentToken::Description && token_type != CommentToken::Eof)
        {
            m_tokenizer.SkipLine();
            token = m_tokenizer.PeekToken();
            token_type = token.GetType();
        }
        if (token_type == CommentToken::Eof) {
            break;
        }

        m_tokenizer.NextToken();
        Token token = m_tokenizer.NextToken();

        if (token.GetType() == CommentToken::OAngle)
        {
            token = m_tokenizer.NextToken();
            std::string name = token.Data();
            Expect(CommentToken::CAngle, token = m_tokenizer.NextToken());

            token = m_tokenizer.NextToken();
            if (token.GetType() == CommentToken::At) {
                ParserDescription(name);
            }
        }

        token = m_tokenizer.PeekToken();
        token_type = token.GetType();
    }
}

std::vector<std::shared_ptr<ParserProp>>
CommentParser::QueryProps(const std::string& name) const
{
    auto itr = m_props.find(name);
    return itr == m_props.end() ? std::vector<std::shared_ptr<ParserProp>>() : itr->second;
}

std::map<CommentToken::Type, std::string>
CommentParser::TokenNames() const
{
	std::map<CommentToken::Type, std::string> names;
	return names;
}

void CommentParser::ParserDescription(const std::string& name)
{
    Token token;
    Expect(CommentToken::String, token = m_tokenizer.NextToken());
    auto type = token.Data();
    if (type == "uniform")
    {
        auto unif = std::make_shared<PropUniform>();

        Expect(CommentToken::String, token = m_tokenizer.NextToken());
        unif->name = token.Data();

        Expect(CommentToken::String, token = m_tokenizer.NextToken());
        unif->type = StringToType(token.Data());

        token = m_tokenizer.NextToken();
        if (token.HasType(CommentToken::String)) {
            unif->display_name = token.Data();
        }

        InsertProp(name, unif);
    }
    else if (type == "function")
    {
        auto func = std::make_shared<PropFunction>();

        Expect(CommentToken::String, token = m_tokenizer.NextToken());
        func->name = token.Data();

        Expect(CommentToken::OParenthesis, token = m_tokenizer.NextToken());
        do {
            token = m_tokenizer.NextToken();
            if (token.HasType(CommentToken::CParenthesis)) {
                break;
            }
            if (!token.HasType(CommentToken::Comma))
            {
                if (token.HasType(CommentToken::Ellipsis)) {
                    func->defalut_params = true;
                } else {
                    func->inputs.push_back(StringToType(token.Data()));
                }
            }
        } while (true);

        Expect(CommentToken::Ret, token = m_tokenizer.NextToken());
        token = m_tokenizer.NextToken();
        func->output = StringToType(token.Data());

        InsertProp(name, func);
    }
    else if (type == "enum")
    {
        auto e = std::make_shared<PropEnum>();

        do {
            Expect(CommentToken::String, token = m_tokenizer.NextToken());
            auto type = token.Data();
            e->types.push_back(type);

            token = m_tokenizer.NextToken();
        } while (token.GetType() != CommentToken::Eol && token.GetType() != CommentToken::Eof);

        InsertProp(name, e);
    }
    else if (type == "default")
    {
        auto def = std::make_shared<PropDefault>();
        def->val = ParserValue();
        InsertProp(name, def);
    }
    else if (type == "region")
    {
        auto region = std::make_shared<PropRegion>();

        Expect(CommentToken::Decimal, token = m_tokenizer.NextToken());
        region->min = token.ToFloat<float>();

        Expect(CommentToken::Comma, token = m_tokenizer.NextToken());

        Expect(CommentToken::Decimal, token = m_tokenizer.NextToken());
        region->max = token.ToFloat<float>();

        InsertProp(name, region);
    }
    else if (type == "export")
    {
        auto exp = std::make_shared<PropExport>();
        InsertProp(name, exp);
    }
    else if (type == "param")
    {
        token = m_tokenizer.NextToken();
        do {
            Expect(CommentToken::OBracket, token);

            auto def = std::make_shared<PropDefault>();

            Expect(CommentToken::String, token = m_tokenizer.NextToken());
            def->name = token.Data();

            Expect(CommentToken::Equal, token = m_tokenizer.NextToken());

            def->val = ParserValue();

            InsertProp(name, def);

            Expect(CommentToken::CBracket, token = m_tokenizer.NextToken());

            token = m_tokenizer.NextToken();
        } while (token.GetType() != CommentToken::Eol && token.GetType() != CommentToken::Eof);
    }
}

std::shared_ptr<Value>
CommentParser::ParserValue()
{
    std::shared_ptr<Value> ret = nullptr;

    auto token = m_tokenizer.NextToken();
    if (token.HasType(CommentToken::Integer))
    {
        auto v = std::make_shared<IntVal>();
        v->x = token.ToInteger<int>();
        ret = v;
    }
    else if (token.HasType(CommentToken::Decimal))
    {
        auto v = std::make_shared<FloatVal>();
        v->x = token.ToFloat<float>();
        ret = v;
    }
    else
    {
        assert(0);
    }

    return ret;
}

void CommentParser::InsertProp(const std::string& key, const std::shared_ptr<ParserProp>& val)
{
    auto itr = m_props.find(key);
    if (itr == m_props.end()) {
        m_props.insert({ key, {val} });
    } else {
        itr->second.push_back(val);
    }
}

}