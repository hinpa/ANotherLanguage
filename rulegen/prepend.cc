#ifndef H_LANG_AST_H
#define H_LANG_AST_H
#include <any>
#include <cstddef>
#include <memory>
#include "../token.hpp"
#include <variant>



struct IStmtVisitor;
struct IExprVisitor;


struct nil {};

typedef std::variant<short, signed, unsigned, int, long, float, double, char, bool, std::string, size_t, nil > ExpressionResult;


extern const char *typeNames[];


struct Expression {
    virtual ExpressionResult accept(IExprVisitor *v) = 0;
    virtual ~Expression() { }
};

struct Stmt {
    virtual void accept(IStmtVisitor *v) = 0;
    virtual ~Stmt() { }
};

enum class ExprType {
    BINARY = 0,
    GROUPING,
    LITERAL,
    UNARY,
    ASSIGNMENT,
    VAR
};


