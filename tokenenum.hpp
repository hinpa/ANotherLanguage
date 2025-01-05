#ifndef H_TOKEN_ENUM_H
#define H_TOKEN_ENUM_H
#include <cstddef>
enum class TokenType {
// two char
    BANG_EQ = 0,
    PLUS_EQ,
    STAR_EQ,
    SLASH_EQ,
    GREAT_EQ,
    LESS_EQ,
    EQ_EQ,
    MINUS_EQ,
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    PLUS,
    MINUS,
    STAR,
    SEMICOL,
    DOT,
    COMMA,
    BANG,
    EQ,
    GREATER,
    LESS,
    SLASH,
// arithmetic types
    SHORT,
    UNSIGNED,
    INT,
    LONG,
    FLOAT,
    DOUBLE,
    CHAR,
    BOOL,
    STRING,
    SIZE_T,
// other kw
    NIL,
    VOID,
    THIS,
    STRUCT,
    CLASS,
    FALSE,
    TRUE,
    IF,
    ELSE,
    FOR,
    WHILE,
    RETURN,

// variable char
    STR_LITERAL,
    NUM_LITERAL,
    DEC_LITERAL,
    IDENTIFIER,


// Special tokens
    PRINT,
    NONE,
    BAD,
    END
};

constexpr size_t getValIndex(TokenType t) {
    if ((size_t)t < (size_t)TokenType::SHORT)
        return -1;
    return (size_t)t - (size_t)TokenType::SHORT;
}

constexpr size_t valIndexMax = (size_t)TokenType::VOID - (size_t)TokenType::SHORT;

constexpr TokenType indexToType(size_t index) {
    constexpr size_t valIndexMax = (size_t)TokenType::VOID;
    if ((size_t)TokenType::SHORT + index > valIndexMax)
        return TokenType::NONE;
    return (TokenType)((size_t)TokenType::SHORT + index);
}

#define ARITHMETIC_TYPES \
    TokenType::SHORT, \
    TokenType::UNSIGNED, \
    TokenType::INT, \
    TokenType::LONG, \
    TokenType::FLOAT, \
    TokenType::DOUBLE, \
    TokenType::CHAR, \
    TokenType::BOOL, \
    TokenType::STRING, \
    TokenType::SIZE_T


#define LITERAL_CASES \
    case TokenType::STR_LITERAL: \
    case TokenType::NUM_LITERAL: \
    case TokenType::DEC_LITERAL:

#define KEYWORD_CASES \
    case TokenType::SHORT: \
    case TokenType::UNSIGNED: \
    case TokenType::INT: \
    case TokenType::LONG: \
    case TokenType::FLOAT: \
    case TokenType::DOUBLE: \
    case TokenType::CHAR: \
    case TokenType::VOID: \
    case TokenType::THIS: \
    case TokenType::STRUCT: \
    case TokenType::CLASS: \
    case TokenType::FALSE: \
    case TokenType::TRUE: \
    case TokenType::IF: \
    case TokenType::ELSE: \
    case TokenType::FOR: \
    case TokenType::WHILE: \
    case TokenType::RETURN: \
    case TokenType::NIL: \
    case TokenType::STR_LITERAL: \
    case TokenType::NUM_LITERAL: \
    case TokenType::DEC_LITERAL: \
    case TokenType::IDENTIFIER: 

#endif
