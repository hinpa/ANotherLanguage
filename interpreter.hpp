#ifndef H_INTERPRET_H
#define H_INTERPRET_H

#include "environment.hpp"
#include "rulegen/ast.hpp"
#include <vector>

#define DECLARE_BOOL_OP(FUNCTION_NAME) bool FUNCTION_NAME(Expression *lhs, Expression *rhs);
#define DECLARE_VAR_OP(FUNCTION_NAME) ExpressionResult FUNCTION_NAME(Expression *lhs, Expression *rhs);

class Interpreter : public IStmtVisitor, public IExprVisitor {
    Environment env;
    template<typename T>
        T tryGet(Expression *e);
    template<typename T>
        T& tryGet( ExpressionResult &res, size_t pos );
    DECLARE_BOOL_OP(comp_less)
    DECLARE_BOOL_OP(comp_less_eq)
    DECLARE_BOOL_OP(comp_greater)
    DECLARE_BOOL_OP(comp_great_eq)
    DECLARE_BOOL_OP(comp_bang_eq)
    DECLARE_BOOL_OP(comp_eq_eq)
    DECLARE_VAR_OP(add)
    DECLARE_VAR_OP(sub)
    DECLARE_VAR_OP(div)
    DECLARE_VAR_OP(mul)
public:
    virtual ExpressionResult visitBinary(Binary&) override;
    virtual ExpressionResult visitGrouping(Grouping&) override;
    virtual ExpressionResult visitLiteral(Literal&) override;
    virtual ExpressionResult visitUnary(Unary&) override;
    virtual ExpressionResult visitVar(Var &) override;
    virtual ExpressionResult visitAssignment(Assignment &) override;
    virtual void visitExpressionStmt(ExpressionStmt&) override;
    virtual void visitPrintStmt(PrintStmt&) override;
    virtual void visitVarStmt(VarStmt&) override;
    void run(std::vector<std::unique_ptr<Stmt> >& stmts) {
        for (auto &p : stmts) {
            p.get()->accept(this);
        }
    }
};

#endif
