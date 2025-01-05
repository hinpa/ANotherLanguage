#include "token.hpp"


Token::Token() : type(TokenType::NONE), lexeme(""), pos(0) {}
Token::Token(TokenType t, const std::string &lex, size_t p) : type(t), lexeme(lex), pos(p) {}


const char *tokenNames[] = { 
    "BANG_EQ",
    "PLUS_EQ",
    "STAR_EQ",
    "SLASH_EQ",
    "GREAT_EQ",
    "LESS_EQ",
    "EQ_EQ",
    "MINUS_EQ",
    "LPAREN",
    "RPAREN",
    "LBRACE",
    "RBRACE",
    "PLUS",
    "MINUS",
    "STAR",
    "SEMICOL",
    "DOT",
    "COMMA",
    "BANG",
    "EQ",
    "GREATER",
    "LESS",
    "SLASH",
    "SHORT",
    "SIGNED",
    "UNSIGNED",
    "INT",
    "LONG",
    "FLOAT",
    "DOUBLE",
    "CHAR",
    "VOID",
    "THIS",
    "STRUCT",
    "CLASS",
    "FALSE",
    "TRUE",
    "IF",
    "ELSE",
    "FOR",
    "WHILE",
    "RETURN",
    "NIL",

// variable char
    "STR_LITERAL",
    "NUM_LITERAL",
    "DEC_LITERAL",
    "IDENTIFIER",


// Special tokens
    "NONE",
    "BAD",
    "END"

};

