#include "environment.hpp"

defineconvertToTYPE(DOUBLE, TokenType::DOUBLE, double);
defineconvertToTYPE(SHORT, TokenType::SHORT, short);
defineconvertToTYPE(INT, TokenType::INT, int);
defineconvertToTYPE(SIGNED, TokenType::INT, int);
defineconvertToTYPE(LONG, TokenType::LONG, long);
defineconvertToTYPE(UNSIGNED, TokenType::UNSIGNED, unsigned);
defineconvertToTYPE(FLOAT, TokenType::FLOAT, float);
defineconvertToTYPE(CHAR, TokenType::CHAR, char);
defineconvertToTYPE(BOOL, TokenType::BOOL, bool);
defineconvertToTYPE(SIZE_T, TokenType::SIZE_T, size_t);

ExpressionResult convert(TokenType t, ExpressionResult &rhs) {
    switch (t) {
        CONVERT_CASES0
        default:
            throw RuntimeError(std::string("Cannot converse ") + typeNames[getValIndex(t)] + " to " + typeNames[rhs.index()], 0);\

    }
}
