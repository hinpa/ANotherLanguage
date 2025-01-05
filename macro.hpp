#ifndef H_UTILITY_MACROS_H
#define H_UTILITY_MACROS_H

#define CREATE_CASE(expr, x) \
    case x: \
        expr;

#define CREATE_COMP_CASE(op, type, x) \
    CREATE_CASE(return std::get<type>(lhsres) op tryGet<type>(rhs), x)

#define CREATE_GET_TYPE_CASE(type, x) \
    CREATE_CASE(return res = type{}, x)

#define CREATE_STRINGIFY_CASE(type, x) \
    CREATE_CASE(return std::to_string(std::get<type>(e)), x)

#define STRINGIFY_CASES \
    CREATE_STRINGIFY_CASE(short, TokenType::SHORT) \
    CREATE_STRINGIFY_CASE(unsigned, TokenType::UNSIGNED) \
    CREATE_STRINGIFY_CASE(long, TokenType::LONG) \
    CREATE_STRINGIFY_CASE(float, TokenType::FLOAT) \
    CREATE_STRINGIFY_CASE(double, TokenType::DOUBLE) \
    CREATE_STRINGIFY_CASE(char, TokenType::CHAR) \
    CREATE_STRINGIFY_CASE(size_t, TokenType::SIZE_T)
    

#define GET_TYPE_CASES \
    CREATE_GET_TYPE_CASE(short, TokenType::SHORT) \
    CREATE_GET_TYPE_CASE(unsigned, TokenType::UNSIGNED) \
    CREATE_GET_TYPE_CASE(int, TokenType::INT) \
    CREATE_GET_TYPE_CASE(long, TokenType::LONG) \
    CREATE_GET_TYPE_CASE(float, TokenType::FLOAT) \
    CREATE_GET_TYPE_CASE(double, TokenType::DOUBLE) \
    CREATE_GET_TYPE_CASE(char, TokenType::CHAR) \
    CREATE_GET_TYPE_CASE(bool, TokenType::BOOL) \
    CREATE_GET_TYPE_CASE(size_t, TokenType::SIZE_T) \
    CREATE_GET_TYPE_CASE(std::string, TokenType::STRING)


#define COMP_CASES(op) \
        CREATE_COMP_CASE(op, double, TokenType::DOUBLE) \
        CREATE_COMP_CASE(op, std::string, TokenType::STRING) \
        CREATE_COMP_CASE(op, bool, TokenType::BOOL)


#define ADD_CASES(op) \
        CREATE_COMP_CASE(op, double, TokenType::DOUBLE) \
        CREATE_COMP_CASE(op, std::string, TokenType::STRING) \

#define OP_CASES(op) \
        CREATE_COMP_CASE(op, double, TokenType::DOUBLE) \

#define EQ_CASES COMP_CASES(op) \
    case TokenType::NIL: \
        return std::get_if<nil>(&lhsres) && std::get_if<nil>(&rhsres);



#define DEFINE_BINARY_OP(TYPE, FUNCTION_NAME, CASES) \
    TYPE Interpreter::FUNCTION_NAME(Expression *lhs, Expression *rhs) { \
    ExpressionResult lhsres = lhs->accept(this); \
    switch (indexToType(lhsres.index())) { \
        CASES \
        default: \
            NO_IMPL \
    } \
}

#define BOOL_BINARY_OP(FUNCTION_NAME, CASES) DEFINE_BINARY_OP(bool, FUNCTION_NAME, CASES)
#define VARIANT_BINARY_OP(FUNCTION_NAME, CASES) DEFINE_BINARY_OP(ExpressionResult, FUNCTION_NAME, CASES)

#endif
