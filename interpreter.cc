#include "interpreter.hpp"
#include "rulegen/ast.hpp"
#include "macro.hpp"
#include "token.hpp"
#include "tokenenum.hpp"
#include "error.hpp"
#include <iostream>
#include <stdexcept>
#include <variant>

class GetPosition : public IExprVisitor {
public:

    virtual ExpressionResult visitBinary(Binary &b) override {
        return b.tok.pos;
    }
    virtual ExpressionResult visitGrouping(Grouping &g) override {
        return g.accept(this);
    }
    virtual ExpressionResult visitLiteral(Literal &l) override {
        return l.tok.pos;
    }
    virtual ExpressionResult visitUnary(Unary &u) override {
        return u.tok.pos;
    }
    virtual ExpressionResult visitVar(Var &v) override {
        return v.name.pos;
    }
    virtual ExpressionResult visitAssignment(Assignment &a) override {
        return a.name.pos;
    }

    size_t visit(Expression *e) {
        return std::get<size_t>(e->accept(this));
    }

};



template<typename T>
T Interpreter::tryGet( Expression *e ) {
    if (!e) 
        return T{};
    ExpressionResult res = e->accept(this);
    try {
        return std::get<T>(res);
    }
    catch (std::bad_variant_access badAccess) {
        throw RuntimeError(std::string("Expected ") + 
                typeNames[ExpressionResult(T{}).index()] + " but got " + typeNames[res.index()]
               ,GetPosition().visit(e)
               ); 
    }
}





BOOL_BINARY_OP(comp_less, COMP_CASES(<))
BOOL_BINARY_OP(comp_less_eq, COMP_CASES(<=))
BOOL_BINARY_OP(comp_greater, COMP_CASES(>))
BOOL_BINARY_OP(comp_great_eq, COMP_CASES(>=))
BOOL_BINARY_OP(comp_eq_eq, COMP_CASES(==))
BOOL_BINARY_OP(comp_bang_eq, COMP_CASES(!=))

VARIANT_BINARY_OP(add, ADD_CASES(+))
VARIANT_BINARY_OP(sub, OP_CASES(-))
VARIANT_BINARY_OP(div, OP_CASES(/))
VARIANT_BINARY_OP(mul, OP_CASES(*))

static bool isTruthy(ExpressionResult e) {
    switch (indexToType(e.index())) {
        case TokenType::BOOL:
            return std::get<bool>(e);
        case TokenType::NIL:
        case TokenType::NONE:
            return false;
        default:
            return true;
    }
}


static std::string exprToString(ExpressionResult e) {
    switch (indexToType(e.index())) {
        case TokenType::STRING:
            return std::get<std::string>(e);
        case TokenType::BOOL:
            return std::get<bool>(e) ? "true" : "false";
        case TokenType::NIL:
            return "NULL";
        STRINGIFY_CASES
        default:
            return "None";
    }
}

void Interpreter::visitPrintStmt(PrintStmt &p) {
    ExpressionResult e = p.expr->accept(this); 
    std::cout << exprToString(e) << std::endl;
    return;
}

void Interpreter::visitExpressionStmt(ExpressionStmt &e) {
    if (!e.expr)
        return;
    e.expr->accept(this); 
    return;
}



void Interpreter::visitVarStmt(VarStmt &v) {
    try {
        ExpressionResult init;
        if (v.expr)
            init = v.expr->accept(this);
        env.define(v.tok, init);
    }
    catch (RuntimeError e) {
        throw RuntimeError(e.what(), v.tok.pos); 
    }
}

ExpressionResult Interpreter::visitAssignment(Assignment &a) {
    try {
        ExpressionResult res = a.expr->accept(this);
        env.assign(a.name.lexeme, res);
        return res;
    } catch (RuntimeError er) {
        throw RuntimeError(er.what(), GetPosition().visit(a.expr.get()));
    }
}

ExpressionResult Interpreter::visitVar(Var &v) {
    try {
        return env.get(v.name.lexeme);
    } catch (RuntimeError e) {
        throw RuntimeError(e.what(), v.name.pos);
    }
}

ExpressionResult Interpreter::visitUnary(Unary &unary) {
    Expression *right = unary.expr.get();
    switch (unary.tok.type) {
        case TokenType::MINUS:
            return -tryGet<double>(right);
        case TokenType::BANG:
            return !isTruthy(right->accept(this));
        default:
            OP_NO_IMPL(unary.tok.lexeme.data())
    }
}


ExpressionResult Interpreter::visitBinary(Binary &bin) {
    Expression *left = bin.left.get();
    Expression *right = bin.right.get();
    switch (bin.tok.type) {
        case TokenType::MINUS:
            return sub(left, right);
        case TokenType::PLUS:
            return add(left, right);
        case TokenType::STAR:
            return mul(left, right);
        case TokenType::SLASH:
            return div(left, right);
        case TokenType::EQ_EQ:
            return comp_eq_eq(left, right);
        case TokenType::LESS_EQ:
            return comp_less_eq(left, right);
        case TokenType::GREAT_EQ:
            return comp_great_eq(left, right);
        case TokenType::LESS:
            return comp_less(left, right);
        case TokenType::GREATER:
            return comp_greater(left, right);
        case TokenType::BANG_EQ:
            return comp_bang_eq(left, right);
        default:
            OP_NO_IMPL(bin.tok.lexeme.data())
    }
}

ExpressionResult Interpreter::visitGrouping(Grouping &group) {
    return group.expr->accept(this);
}

ExpressionResult Interpreter::visitLiteral(Literal &lit) { // "fdsfsdf" 123123.1213 231 12313.1f 'c'
    switch (lit.tok.type) {
        case TokenType::STR_LITERAL:
            return lit.tok.lexeme;
        case TokenType::NUM_LITERAL: {
            double db = std::stod(lit.tok.lexeme);
            return db;
        }
        case TokenType::DEC_LITERAL:
            NO_IMPL
        default:
            throw std::runtime_error("Expected a literal value");
    }
}
