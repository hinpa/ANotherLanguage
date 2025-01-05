

struct GetType : public IExprVisitor {
    virtual ExpressionResult visitBinary(Binary&) { return (size_t)ExprType::BINARY; };
    virtual ExpressionResult visitGrouping(Grouping&) { return (size_t)ExprType::GROUPING; };
    virtual ExpressionResult visitLiteral(Literal&) { return (size_t)ExprType::LITERAL; };
    virtual ExpressionResult visitUnary(Unary&) { return (size_t)ExprType::UNARY; };
    virtual ExpressionResult visitAssignment(Assignment&) { return (size_t)ExprType::ASSIGNMENT; };
    virtual ExpressionResult visitVar(Var&) { return (size_t)ExprType::VAR; };
    ExprType getType(Expression* e)
    {
        return (ExprType)std::get<size_t>(e->accept(this));
    }
};


