#include "kw.hpp"
#include "tokenenum.hpp"

namespace kw {

    const keyword_t KEYWORDS[] = {
        {"!=",TokenType::BANG_EQ},
        {"+=",TokenType::PLUS_EQ},
        {"*=",TokenType::STAR_EQ},
        {"/=",TokenType::SLASH_EQ},
        {">=",TokenType::GREAT_EQ},
        {"<=",TokenType::LESS_EQ},
        {"==",TokenType::EQ_EQ},
        {"-=",TokenType::MINUS_EQ},
        {"(",TokenType::LPAREN},
        {")",TokenType::RPAREN},
        {"{",TokenType::LBRACE},
        {"}",TokenType::RBRACE},
        {"+",TokenType::PLUS},
        {"-",TokenType::MINUS},
        {"*",TokenType::STAR},
        {";",TokenType::SEMICOL},
        {".",TokenType::DOT},
        {",",TokenType::COMMA},
        {"!",TokenType::BANG},
        {"=",TokenType::EQ},
        {">",TokenType::GREATER},
        {"<",TokenType::LESS},
        {"/",TokenType::SLASH},
        {"string", TokenType::STRING},
        {"short",TokenType::SHORT},
        {"unsigned",TokenType::UNSIGNED},
        {"int",TokenType::INT},
        {"long",TokenType::LONG},
        {"float",TokenType::FLOAT},
        {"double",TokenType::DOUBLE},
        {"char",TokenType::CHAR},
        {"void",TokenType::VOID},
        {"this",TokenType::THIS},
        {"struct",TokenType::STRUCT},
        {"class",TokenType::CLASS},
        {"false",TokenType::FALSE},
        {"true",TokenType::TRUE},
        {"if",TokenType::IF},
        {"else",TokenType::ELSE},
        {"for",TokenType::FOR},
        {"while",TokenType::WHILE},
        {"return",TokenType::RETURN},
        {"null", TokenType::NIL},
        {"print", TokenType::PRINT}
    };


    const size_t kwsize = sizeof(KEYWORDS)/sizeof(std::pair<char*,TokenType>);

    const size_t npos = -1;
}

