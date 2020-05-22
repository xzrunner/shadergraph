#include "shadergraph/BlockParser.h"
#include "shadergraph/Variant.h"
#include "shadergraph/ValueImpl.h"
#include "shadergraph/CommentParser.h"
#include "shadergraph/ParserUtility.h"

#include <lexer/Exception.h>

namespace shadergraph
{

//////////////////////////////////////////////////////////////////////////
// class BlockTokenizer
//////////////////////////////////////////////////////////////////////////

BlockTokenizer::BlockTokenizer(const std::string& str)
    : lexer::Tokenizer<BlockToken::Type>(str.c_str(), str.c_str() + str.length(), "\"", '\\')
    , m_skip_eol(true)
{
}

lexer::Tokenizer<BlockToken::Type>::Token
BlockTokenizer::EmitToken()
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
                    const char* e = DiscardUntil("\n\r");
                    return Token(BlockToken::Comment, c, e, Offset(c), start_line, start_column);
                }
                else
                {
                    return Token(BlockToken::Div, c, c + 1, Offset(c), start_line, start_column);
                }
                break;
            case '{':
                Advance();
                return Token(BlockToken::OBrace, c, c+1, Offset(c), start_line, start_column);
            case '}':
                Advance();
                return Token(BlockToken::CBrace, c, c+1, Offset(c), start_line, start_column);
            case '(':
                Advance();
                return Token(BlockToken::OParenthesis, c, c+1, Offset(c), start_line, start_column);
            case ')':
                Advance();
                return Token(BlockToken::CParenthesis, c, c+1, Offset(c), start_line, start_column);
            case '[':
                Advance();
                return Token(BlockToken::OBracket, c, c+1, Offset(c), start_line, start_column);
            case ']':
                Advance();
                return Token(BlockToken::CBracket, c, c+1, Offset(c), start_line, start_column);
            case ',':
                Advance();
                return Token(BlockToken::Comma, c, c + 1, Offset(c), start_line, start_column);
            case '.':
                Advance();
                return Token(BlockToken::Dot, c, c + 1, Offset(c), start_line, start_column);
            case '#':
            {
                const char* e = DiscardUntil("\n\r");
                return Token(BlockToken::Pound, c, e, Offset(c), start_line, start_column);
            }
            case ':':
                Advance();
                return Token(BlockToken::Colon, c, c + 1, Offset(c), start_line, start_column);
            case '?':
                Advance();
                return Token(BlockToken::Question, c, c + 1, Offset(c), start_line, start_column);
            case '+':
                Advance();
                return Token(BlockToken::Add, c, c + 1, Offset(c), start_line, start_column);
            case '-':
                Advance();
                return Token(BlockToken::Sub, c, c + 1, Offset(c), start_line, start_column);
            case '*':
                Advance();
                return Token(BlockToken::Mul, c, c + 1, Offset(c), start_line, start_column);
            case '=':
                Advance();
                return Token(BlockToken::Equal, c, c + 1, Offset(c), start_line, start_column);
            case '"': { // quoted string
                Advance();
                c = CurPos();
                const char* e = ReadQuotedString('"', "\n}");
                return Token(BlockToken::String, c, e, Offset(c), start_line, start_column);
            }
            case '\n':
                if (!m_skip_eol) {
                    Advance();
                    return Token(BlockToken::Eol, c, c+1, Offset(c), start_line, start_column);
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
            default: { // whitespace, integer, decimal or word
                const char* e = ReadInteger(NumberDelim());
				if (e != nullptr) {
					return Token(BlockToken::Integer, c, e, Offset(c), start_line, start_column);
				}

                e = ReadDecimal(NumberDelim());
				if (e != nullptr) {
					return Token(BlockToken::Decimal, c, e, Offset(c), start_line, start_column);
				}

                static const std::string str_separator(Whitespace() + ";{}[](),.#:?+-*/=");
                e = ReadUntil(str_separator);
				if (e == nullptr) {
					throw lexer::ParserException(start_line, start_column, "Unexpected character: " + std::string(c, 1));
				}

                return Token(BlockToken::String, c, e, Offset(c), start_line, start_column);
            }
        }
    }
    return Token(BlockToken::Eof, nullptr, nullptr, Length(), Line(), Column());
}

const std::string& BlockTokenizer::NumberDelim()
{
    static const std::string number_delim(Whitespace() + ")]};");
    return number_delim;
}

//////////////////////////////////////////////////////////////////////////
// class BlockParser
//////////////////////////////////////////////////////////////////////////

BlockParser::BlockParser(const std::string& str)
    : m_tokenizer(BlockTokenizer(str))
{
}

void BlockParser::Parse()
{
    Token token = m_tokenizer.PeekToken();
    BlockToken::Type token_type = token.GetType();
    while (token_type != BlockToken::Eof)
    {
        bool advanced = false;
        if (token_type == BlockToken::String)
        {
            auto str = token.Data();
            if (str == "struct") {
                ParseStruct();
                advanced = true;
            } else if (str == "uniform") {
                ParseUniform();
                advanced = true;
            } else {
                auto var = ParseVariable();
                advanced = true;
            }
        }
        else if (token_type == BlockToken::Comment)
        {
            auto str = token.Data();
            CommentParser parser(str);
            parser.Parse(m_desc);
        }

        if (!advanced) {
            m_tokenizer.NextToken();
        }

        token = m_tokenizer.PeekToken();
        token_type = token.GetType();
    }
}

void BlockParser::ParseStruct()
{
    Struct s;

    Token token;

    // struct
    Expect(BlockToken::String, token = m_tokenizer.NextToken());
    assert(token.Data() == "struct");

    // name
    Expect(BlockToken::String, token = m_tokenizer.NextToken());
    s.name = token.Data();

    // members
    Expect(BlockToken::OBrace, token = m_tokenizer.NextToken());
    token = m_tokenizer.PeekToken();
    while (token.GetType() != BlockToken::CBrace)
    {
        s.vars.push_back(ParseVariable());
        token = m_tokenizer.PeekToken();
    }

    m_structs.push_back(s);
}

void BlockParser::ParseUniform()
{
    Token token;
    Expect(BlockToken::String, token = m_tokenizer.NextToken());
    assert(token.Data() == "uniform");

    Variant u_var;
    u_var.type = VarType::Uniform;

    auto var = ParseVariable();
    u_var.name = var.name;

    auto unif = std::make_shared<UniformVal>();
    unif->desc = m_desc;
    m_desc.clear();
    unif->var = var;
    u_var.val = unif;

    m_uniforms.push_back(u_var);
}

Variant BlockParser::ParseVariable()
{
    Variant var;

    Token token;
    Expect(BlockToken::String, token = m_tokenizer.NextToken());
    // todo: const
    if (token.Data() == "const") {
        var.is_const = true;
        Expect(BlockToken::String, token = m_tokenizer.NextToken());
    }
    var.type = StringToType(token.Data());

    Expect(BlockToken::String, token = m_tokenizer.NextToken());
    var.name = token.Data();

    // array
    token = m_tokenizer.PeekToken();
    if (token.GetType() == BlockToken::OBracket)
    {
        m_tokenizer.NextToken();    // skip open bracket
        Expect(BlockToken::Integer, token = m_tokenizer.NextToken());

        auto val = std::make_shared<ArrayVal>();
        const int num = token.ToInteger<int>();
        val->type = var.type;
        val->items.resize(num);
        var.val = val;

        var.type = VarType::Array;

        Expect(BlockToken::CBracket, token = m_tokenizer.NextToken());
    }
    // function
    else if (token.GetType() == BlockToken::OParenthesis)
    {
        m_tokenizer.NextToken();    // skip open parenthesis

        auto val = std::make_shared<FunctionVal>();

        val->desc = m_desc;
        m_desc.clear();

        val->output.type = var.type;
        val->output.name = "ret";

        var.type = VarType::Function;

        token = m_tokenizer.NextToken();
        while (!token.HasType(BlockToken::CParenthesis))
        {
            Variant var;

            var.type = StringToType(token.Data());
            assert(var.type != VarType::Invalid);

            Expect(BlockToken::String, token = m_tokenizer.NextToken());
            var.name = token.Data();

            val->inputs.push_back(var);

            token = m_tokenizer.NextToken();
            if (token.HasType(BlockToken::Comma)) {
                token = m_tokenizer.NextToken();
            }
        }
        var.val = val;

        Expect(BlockToken::OBrace, token = m_tokenizer.NextToken());

        auto start_token = m_tokenizer.NextToken();
        auto start = start_token.Begin();

        int level = 0;
        do {
            if (token.HasType(BlockToken::OBrace)) {
                ++level;
            } else if (token.HasType(BlockToken::CBrace)) {
                --level;
                if (level == 0) {
                    break;
                }
            }
            token = m_tokenizer.NextToken();
        } while (level != 0);

        std::string body = "    ";
        if (token.GetType() == BlockToken::Eof) {
            body += std::string(start);
        } else {
            body += std::string(start, token.Begin());
        }

        val->code = body;

        Variant func_var;
        func_var.type = VarType::Function;
        func_var.name = var.name;
        func_var.val = val;
        m_funcs.push_back(var);
    }
    // assign
    else if (token.GetType() == BlockToken::Equal)
    {
        m_tokenizer.NextToken();    // skip equal
        switch (var.type)
        {
        case VarType::Int:
        {
            Expect(BlockToken::Integer, token = m_tokenizer.NextToken());
            auto val = std::make_shared<IntVal>();
            val->x = token.ToInteger<int>();
            var.val = val;
        }
            break;
        case VarType::Float:
        {
            Expect(BlockToken::Decimal, token = m_tokenizer.NextToken());
            auto val = std::make_shared<FloatVal>();
            val->x = token.ToFloat<float>();
            var.val = val;
        }
            break;
        default:
            assert(0);
        }
    }
    // default value
    else
    {
        switch (var.type)
        {
        case VarType::Int:
            var.val = std::make_shared<IntVal>();
            break;
        case VarType::Float:
            var.val = std::make_shared<FloatVal>();
            break;
        case VarType::Float3:
            var.val = std::make_shared<Float3Val>();
            break;
        default:
            assert(0);
        }
    }

    assert(var.type != VarType::Invalid);

    return var;
}

std::map<BlockToken::Type, std::string>
BlockParser::TokenNames() const
{
	using namespace BlockToken;

	std::map<BlockToken::Type, std::string> names;
	//names[Integer]      = "integer";
	//names[Decimal]      = "decimal";
	//names[String]       = "string";
	//names[OParenthesis] = "'('";
	//names[CParenthesis] = "')'";
	//names[OBrace]       = "'{'";
	//names[CBrace]       = "'}'";
	//names[OBracket]     = "'['";
	//names[CBracket]     = "']'";
 //   names[Comma]        = "','";
 //   names[Dot]          = "'.'";
	//names[Comment]      = "comment";
	//names[Eof]          = "end of file";

	return names;
}

}