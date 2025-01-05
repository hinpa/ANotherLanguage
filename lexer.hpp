#ifndef H_LEXER_H
#define H_LEXER_H
#include "error.hpp"
#include "token.hpp"
#include <vector>

class Lexer { 
public:
    typedef std::vector<Token> container ;
private:
    container tokens;
    std::string source;
    size_t cursor;
private:
    void trim();
    char peek();
    void consume();
    size_t map(const char *substr);
    size_t match(const std::string &s);
    inline bool isEOF( );
    Maybe<std::string> tryEatNumber(const char *substr);
    Maybe<std::string> tryEatStringLiteral(const char *substr);
    Maybe<std::string> tryEatID(const char *substr);
    void ErrorHandler(size_t pos, Maybe<std::string> e);
    Token scanToken( );
public:
    Lexer();
    Lexer(std::string s);
    Lexer(const Lexer &lexer) = delete;
    Lexer(Lexer &&) ;
public:
    void setSource( std::string s );
    container &&scanSource();
    container &&getTokens();
public:
    Lexer &operator=(const Lexer &) = delete;
    Lexer &operator=(Lexer &&);
};

#endif
