#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <string>
#include <string_view>
#include <vector>

#include "error.hpp"
#include "token.hpp"
#include "kw.hpp"
#include "lexer.hpp"
#include "tokenenum.hpp"
#include "util.hpp"




size_t Lexer::map(const char *substr) {
    for (size_t i = 0; i != kw::kwsize; ++i)
        if (strings::starts_with(substr, kw::KEYWORDS[i].lexeme))
            return i;
    return kw::npos;
}
size_t Lexer::match(const std::string &s) {
    for (size_t i = 0; i != kw::kwsize; ++i)
        if (s == kw::KEYWORDS[i].lexeme)
            return i;
    return kw::npos; 
}
inline bool Lexer::isEOF( ) {
    return cursor >= source.size();
}
Maybe<std::string> Lexer::tryEatNumber(const char *substr) {
    if (!substr || !substr[0])
        return {ErrorCode::FAILURE};
    bool isWhole = substr[0] != '.';
    size_t i = 1;
    if (isWhole && !isdigit(substr[0]))
        return {ErrorCode::FAILURE};
    while (substr[i] && !isspace(substr[i]) && (isdigit(substr[i]) || substr[i] == '.'))
    {
        if (substr[i] == '.')
        {
            if (!isWhole)
                return {ErrorCode::MALFORMED_LITERAL};
            isWhole = false;
        }
        ++i;
    }
    std::string ret(substr,i); 
    return {ret, ErrorCode::SUCCESS};
}
Maybe<std::string> Lexer::tryEatStringLiteral(const char *substr) {
    if (!substr || !substr[0] || substr[0] != '\"')
        return {ErrorCode::FAILURE};
    size_t i = 1;
    while (substr[i] && substr[i] != '\"')
    {
        ++i;
    }
    if (substr[i] != '\"')
        return {ErrorCode::MALFORMED_LITERAL};
    std::string ret(substr + 1, i - 1); 
    return {ret, SUCCESS};
}
Maybe<std::string> Lexer::tryEatID(const char *substr) {
    auto isValid = [](char c, bool first = false) -> bool {
        return (!first && isdigit(c)) || isalpha(c) || c == '_';
    };
    if (!substr || !substr[0] || !isValid(substr[0], true))
        return {ErrorCode::FAILURE};
    size_t i = 0;
    while (substr[i] && isValid(substr[i]))
        ++i;
    std::string ret(substr, i);
    return {ret, ErrorCode::SUCCESS};
}
void Lexer::ErrorHandler(size_t pos, Maybe<std::string> e) {
    std::string errorMessage;
    switch (e.error) {
        case ErrorCode::MALFORMED_LITERAL:
            errorMessage = "Malformed literal ";
            break;
        case ErrorCode::NO_TOK:
            errorMessage = "Invalid token ";
            break;
        default:
            return;
    }
    throw LexerError(errorMessage, cursor);
}
Token Lexer::scanToken( ) {
    if (isEOF())
        return {TokenType::NONE, "", cursor};
    Token tok;

    Maybe<std::string> lexeme;
    const char *data = source.data() + cursor;
    auto lambda = [this](Error e) {
        ErrorHandler(this->cursor, e);
        return e;
    };
    if (!lambda((lexeme = tryEatStringLiteral(data)).error)) { // special case
        tok.lexeme = std::move(lexeme.val);
        tok.type = TokenType::STR_LITERAL;
        tok.pos = cursor + 1;
        cursor += 2;
    }
    else if (!lambda(((lexeme = tryEatNumber(data)).error))) {
        tok.lexeme = std::move(lexeme.val);
        tok.type = TokenType::NUM_LITERAL;
        tok.pos = cursor;
    }
    else if (!lambda((lexeme = tryEatID(data)).error)) {
        tok.lexeme = std::move(lexeme.val);
        tok.type = TokenType::IDENTIFIER;
        size_t i = match(tok.lexeme);
        if (i != kw::npos)
        {
            const kw::keyword_t &kword = kw::KEYWORDS[i];
            tok.lexeme = kword.lexeme;
            tok.type = kword.type;
        }
        tok.pos = cursor;
    }

    size_t mappedValue = -1;
    if (tok.type == TokenType::NONE)
        mappedValue = map(data); 



    if (mappedValue != kw::npos) {
        const kw::keyword_t &kword = kw::KEYWORDS[mappedValue];
        tok.lexeme = kword.lexeme;
        tok.type = kword.type;
        tok.pos = cursor;
    }


    if (tok.type == TokenType::NONE) {
        ErrorHandler(cursor, {ErrorCode::NO_TOK});
    }

    cursor += tok.lexeme.size();

    return tok; 

}
void Lexer::setSource( std::string s ) {
    source = s;         
    tokens.clear();
}
char Lexer::peek() {
    return isEOF() ? '\0' : source[cursor];
}
void Lexer::trim() {
    cursor = strings::trimSpace(source.data() + cursor) - source.data();
}
Lexer::container &&Lexer::scanSource() {
    while (cursor < source.size())
    {
        trim();
        tokens.push_back(scanToken());
        trim();
    }
    tokens.push_back({TokenType::END, "", cursor});
    return std::move(tokens);
}

Lexer::container &&Lexer::getTokens() {
    return std::move(tokens);
}

Lexer::Lexer() : cursor(0){ }
Lexer::Lexer( std::string s ) : source(s), cursor(0){ }
Lexer::Lexer(Lexer &&lex) {
    tokens = std::move(lex.tokens);
}
Lexer& Lexer::operator=(Lexer &&lex) {
    tokens = std::move(lex.tokens);
    return *this;
}
