#ifndef H_KEYWORDS_H
#define H_KEYWORDS_H
#include "token.hpp"

namespace kw {
    struct keyword_t {
        const char *lexeme;
        TokenType type;
    };
    extern const keyword_t KEYWORDS[];
    extern const size_t kwsize;
    extern const size_t npos;
}
#endif
