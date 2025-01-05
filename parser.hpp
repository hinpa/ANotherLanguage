#ifndef H_PARSER_H
#define H_PARSER_H
#include "rulegen/ast.hpp"
#include "tokenenum.hpp"
#include <algorithm>
#include <vector>

class Parser { 
    std::vector<Token> tokens;
    size_t cursor;
public:
    typedef std::unique_ptr<Expression> ExprNode;
    typedef std::unique_ptr<Stmt> StmtNode;
private: // helper functions
    Token &advance();
    bool isEOF();
    bool check(TokenType tokType);
    Token &peek();
    bool match(std::initializer_list<TokenType> l);
    Token &getPrevToken();
    Token &consume(TokenType tokType, const std::string &message);
    void synchronize();
private:
    StmtNode declaration();
    StmtNode varDeclaration();
    StmtNode statement();
    StmtNode printStmt();
    StmtNode expressionStmt();
    ExprNode expression();
    ExprNode assignment();
    ExprNode equality();
    ExprNode comparison();
    ExprNode term();
    ExprNode factor();
    ExprNode unary();
    ExprNode primary();
public:
    Parser();
    Parser(std::vector<Token> &&toks);
public:
    void setTokens(std::vector<Token> &&toks);
    std::vector<StmtNode> parse();
};

#endif
