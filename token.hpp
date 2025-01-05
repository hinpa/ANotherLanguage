#ifndef H_TOKENS_H
#define H_TOKENS_H
#include <cstddef>
#include <string>
#include "tokenenum.hpp"


struct Token {
    TokenType type;
    std::string lexeme;
    size_t pos;
    Token();
    Token(TokenType t, const std::string &lex, size_t p);
};

extern const char *tokenNames[];


#endif
