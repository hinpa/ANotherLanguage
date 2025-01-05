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



struct Binary;
struct Grouping;
struct Literal;
struct Unary;
struct Assignment;
struct Var;

struct IExprVisitor {
    virtual ExpressionResult visitBinary(Binary&) = 0;
    virtual ExpressionResult visitGrouping(Grouping&) = 0;
    virtual ExpressionResult visitLiteral(Literal&) = 0;
    virtual ExpressionResult visitUnary(Unary&) = 0;
    virtual ExpressionResult visitAssignment(Assignment&) = 0;
    virtual ExpressionResult visitVar(Var&) = 0;
};


struct ExpressionStmt;
struct PrintStmt;
struct VarStmt;
struct IStmtVisitor {
    virtual void visitExpressionStmt(ExpressionStmt&) = 0;
    virtual void visitPrintStmt(PrintStmt&) = 0;
    virtual void visitVarStmt(VarStmt&) = 0;
};
struct Binary : public Expression {
    std::unique_ptr<Expression> left;
    Token tok;
    std::unique_ptr<Expression> right;
    Binary(std::unique_ptr<Expression>&& left_, Token tok_, std::unique_ptr<Expression>&& right_) : left(std::move(left_)) , tok(tok_) , right(std::move(right_))  {} 
    virtual ExpressionResult accept(IExprVisitor *v) { return v->visitBinary(*this); }
~Binary(){};
};

struct Grouping : public Expression {
    std::unique_ptr<Expression> expr;
    Grouping(std::unique_ptr<Expression>&& expr_) : expr(std::move(expr_))  {} 
    virtual ExpressionResult accept(IExprVisitor *v) { return v->visitGrouping(*this); }
~Grouping(){};
};

struct Literal : public Expression {
    Token tok;
    Literal(Token tok_) : tok(tok_)  {} 
    virtual ExpressionResult accept(IExprVisitor *v) { return v->visitLiteral(*this); }
~Literal(){};
};

struct Unary : public Expression {
    Token tok;
    std::unique_ptr<Expression> expr;
    Unary(Token tok_, std::unique_ptr<Expression>&& expr_) : tok(tok_) , expr(std::move(expr_))  {} 
    virtual ExpressionResult accept(IExprVisitor *v) { return v->visitUnary(*this); }
~Unary(){};
};

struct Assignment : public Expression {
    Token name;
    std::unique_ptr<Expression> expr;
    Assignment(Token name_, std::unique_ptr<Expression>&& expr_) : name(name_) , expr(std::move(expr_))  {} 
    virtual ExpressionResult accept(IExprVisitor *v) { return v->visitAssignment(*this); }
~Assignment(){};
};

struct ExpressionStmt : public Stmt {
    std::unique_ptr<Expression> expr;
    ExpressionStmt(std::unique_ptr<Expression>&& expr_) : expr(std::move(expr_))  {} 
    virtual void accept(IStmtVisitor *v) { return v->visitExpressionStmt(*this); }
~ExpressionStmt(){};
};

struct PrintStmt : public Stmt {
    std::unique_ptr<Expression> expr;
    PrintStmt(std::unique_ptr<Expression>&& expr_) : expr(std::move(expr_))  {} 
    virtual void accept(IStmtVisitor *v) { return v->visitPrintStmt(*this); }
~PrintStmt(){};
};

struct VarStmt : public Stmt {
    Token tok;
    std::unique_ptr<Expression> expr;
    VarStmt(Token tok_, std::unique_ptr<Expression>&& expr_) : tok(tok_) , expr(std::move(expr_))  {} 
    virtual void accept(IStmtVisitor *v) { return v->visitVarStmt(*this); }
~VarStmt(){};
};

struct Var : public Expression {
    Token name;
    Var(Token name_) : name(name_)  {} 
    virtual ExpressionResult accept(IExprVisitor *v) { return v->visitVar(*this); }
~Var(){};
};


#endif