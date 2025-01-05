#ifndef H_ENV_H
#define H_ENV_H

#include "error.hpp"
#include "rulegen/ast.hpp"
#include "token.hpp"
#include "tokenenum.hpp"
#include "macro.hpp"
#include <unordered_map>

#define CREATE_CONVERT_CASE(totype, type, typename) \
    case typename: \
        return ExpressionResult((totype)(std::get<type>(e)));


#define CONVERT_CASES(toType) \
    CREATE_CONVERT_CASE(toType, short, TokenType::SHORT) \
    CREATE_CONVERT_CASE(toType, unsigned, TokenType::UNSIGNED) \
    CREATE_CONVERT_CASE(toType, int, TokenType::INT) \
    CREATE_CONVERT_CASE(toType, long, TokenType::LONG) \
    CREATE_CONVERT_CASE(toType, float, TokenType::FLOAT) \
    CREATE_CONVERT_CASE(toType, double, TokenType::DOUBLE) \
    CREATE_CONVERT_CASE(toType, char, TokenType::CHAR) \
    CREATE_CONVERT_CASE(toType, bool, TokenType::BOOL) \
    CREATE_CONVERT_CASE(toType, size_t, TokenType::SIZE_T) \

#define CREATE_CONVERT_CASE0(type, typename) \
    case typename: \
        return convertTo##type(rhs);

#define CONVERT_CASES0 \
    CREATE_CONVERT_CASE0(SHORT, TokenType::SHORT) \
    CREATE_CONVERT_CASE0(UNSIGNED, TokenType::UNSIGNED) \
    CREATE_CONVERT_CASE0(INT, TokenType::INT) \
    CREATE_CONVERT_CASE0(LONG, TokenType::LONG) \
    CREATE_CONVERT_CASE0(FLOAT, TokenType::FLOAT) \
    CREATE_CONVERT_CASE0(DOUBLE, TokenType::DOUBLE) \
    CREATE_CONVERT_CASE0(CHAR, TokenType::CHAR) \
    CREATE_CONVERT_CASE0(BOOL, TokenType::BOOL) \
    CREATE_CONVERT_CASE0(SIZE_T, TokenType::SIZE_T) \

#define defineconvertToTYPE(func_name, typename, type) \
    ExpressionResult convertTo##func_name(const ExpressionResult &e) { \
        switch (indexToType(e.index())) { \
            CONVERT_CASES(type) \
            default: \
                throw RuntimeError(std::string("Cannot converse ") + typeNames[e.index()] + " to " + typeNames[getValIndex(typename)], 0);\
        }\
    }

#define declareconvertToTYPE(func_name) \
    ExpressionResult convertTo##func_name(const ExpressionResult &e);


declareconvertToTYPE(DOUBLE);
declareconvertToTYPE(SHORT);
declareconvertToTYPE(LONG);
declareconvertToTYPE(UNSIGNED);
declareconvertToTYPE(FLOAT);
declareconvertToTYPE(CHAR);
declareconvertToTYPE(BOOL);
declareconvertToTYPE(SIZE_T);

ExpressionResult convert(TokenType t, ExpressionResult &rhs);


class Environment {

    struct Variable {
        TokenType type;
        ExpressionResult value;
        Variable() : type(TokenType::NONE), value() {}
        Variable(TokenType t) : type(t), value() {}
        Variable(TokenType t, const ExpressionResult &val) : type(t), value(val) {} 
        Variable(const Variable &var) : type(var.type), value(var.value) {

        }
        Variable &operator=(const Variable &var) {
            value = var.value;
            type = var.type;
            return *this;
        }
        Variable &operator=(Variable &&var) {
            value = std::move(var.value);
            type = var.type;
            return *this;
        }
    };
    std::unordered_map<std::string, Variable> map;
public:
    void define(Token &name, ExpressionResult value) {
        if (map.find(name.lexeme) != map.end())
            throw RuntimeError(name.lexeme + " is redefined", 0);
        if (getValIndex(name.type) != value.index())
            value = convert(name.type, value);
        map[name.lexeme] = Variable(name.type, value);

    }
    ExpressionResult get(std::string name) {
        if (map.find(name) == map.end())
            throw RuntimeError(name + " is undeclared", 0);
        return map[name].value; 
    }
    void assign(std::string name, ExpressionResult result) {
        if (map.find(name) == map.end())
            throw RuntimeError(name + " is undeclared", 0);
        Variable &var = map[name];
        if (getValIndex(var.type) != result.index())
            result = convert(var.type, result);
        map[name].value = result;
    }
};

#endif
