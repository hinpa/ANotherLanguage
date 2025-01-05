#include "parser.hpp"
#include "rulegen/ast.hpp"
#include "token.hpp"
#include "tokenenum.hpp"
#include "error.hpp"
#include <algorithm>
#include <memory>




void Parser::setTokens(std::vector<Token> &&toks) {
    tokens = toks;
}

Token &Parser::advance() {
    if (cursor >= tokens.size())
        return tokens.back();
    Token &tok = tokens[cursor];
    ++cursor;
    return tok;
}
bool Parser::isEOF() {
    return cursor >= tokens.size();
}
bool Parser::check(TokenType tokType) {
    return cursor >= tokens.size() ? false : tokType == tokens[cursor].type;
}
Token &Parser::peek() {
    return tokens[cursor];
}
bool Parser::match(std::initializer_list<TokenType> l) {
    for (TokenType tokType : l) {
        if (check(tokType)) {
            advance();
            return true;
        }
    }
    return false;
}
Token &Parser::getPrevToken() {
    return tokens[cursor - 1];
}

Parser::StmtNode Parser::declaration() {
    if (match({ARITHMETIC_TYPES})) return varDeclaration();

    return statement();
}

Parser::StmtNode Parser::varDeclaration() {
    Token &type = getPrevToken();
    Token &tok = consume(TokenType::IDENTIFIER, "Expected id");
    ExprNode init = nullptr;
    if (match({TokenType::EQ}))
        init = expression();
    consume(TokenType::SEMICOL, "Expected ; after expression");
    tok.type = type.type;
    return std::make_unique<VarStmt>(tok, std::move(init));
}

Parser::StmtNode Parser::statement() {
    if (match({TokenType::PRINT})) return printStmt();
    return expressionStmt();
}

Parser::StmtNode Parser::printStmt() {
    if (peek().type != TokenType::LPAREN) throw ParseError("Expected ; after expression", peek().pos);
    ExprNode e = expression();    
    consume(TokenType::SEMICOL, "Expected ; after expression");
    return std::make_unique<PrintStmt>(std::move(e));
}
Parser::StmtNode Parser::expressionStmt() {
    ExprNode e = expression();
    consume(TokenType::SEMICOL, "Expected ; after expression");
    return std::make_unique<ExpressionStmt>(std::move(e));
}

Parser::ExprNode Parser::expression() {
    return assignment();
}

Parser::ExprNode Parser::assignment() {
    ExprNode right = equality();
    if (match({TokenType::EQ})) {
        Token &equals = getPrevToken(); // for error purposes
        ExprNode val = assignment();
        if (GetType().getType(right.get()) == ExprType::VAR) {
            Token name = ((Var*)right.get())->name;
            return std::make_unique<Assignment>(name, std::move(val));
        }
        
        throw ParseError("left-hand side is rvalue", equals.pos);
    }
    return right;
}

Parser::ExprNode Parser::equality() {
    Parser::ExprNode expr = comparison();
    while (match({TokenType::EQ_EQ, TokenType::BANG_EQ}))
    {
        Token matchedTok = getPrevToken();
        Parser::ExprNode right = comparison(); 
        expr = std::make_unique<Binary>(std::move(expr), matchedTok, std::move(right));
    }

    return expr;
}
Parser::ExprNode Parser::comparison() {
    Parser::ExprNode expr = term();
    while (match({TokenType::LESS, TokenType::LESS_EQ, TokenType::GREATER, TokenType::GREAT_EQ}))
    {
        Token matchedTok = getPrevToken();
        Parser::ExprNode right = term();
        expr = std::make_unique<Binary>(std::move(expr), matchedTok, std::move(right));
    }
    return expr; 
}
Parser::ExprNode Parser::term() {
    Parser::ExprNode expr = factor();
    while (match({TokenType::PLUS, TokenType::MINUS}))
    {
        Token matchedTok = getPrevToken();
        Parser::ExprNode right = factor();
        expr = std::make_unique<Binary>(std::move(expr), matchedTok, std::move(right));
    }
    return expr; 
}
Parser::ExprNode Parser::factor() {
    Parser::ExprNode expr = unary();
    while (match({TokenType::STAR, TokenType::SLASH}))
    {
        Token matchedTok = getPrevToken();
        Parser::ExprNode right = unary();
        expr = std::make_unique<Binary>(std::move(expr), matchedTok, std::move(right));
    }
    return expr; 
}
Token &Parser::consume(TokenType tok, const std::string &message) {
    if (check(tok)) return advance();
    throw ParseError(message, peek().pos);
}
Parser::ExprNode Parser::unary() {
    if (match({TokenType::MINUS, TokenType::BANG, TokenType::PLUS})) {
        Token matchedTok = getPrevToken();
        Parser::ExprNode right = unary();
        if (!right)
            throw int();
        return std::make_unique<Unary>(matchedTok, std::move(right));
    }
    return primary();
}
Parser::ExprNode Parser::primary() {
    if (isEOF())
        return NULL;
    switch (peek().type) {
        LITERAL_CASES
        {
            return std::make_unique<Literal>(advance());
        }
        case TokenType::IDENTIFIER:
        {
            return std::make_unique<Var>(advance());
        }
        case TokenType::SEMICOL:
        case TokenType::END: {
            return NULL;
        }
        case TokenType::LPAREN: {
            advance();
            Parser::ExprNode expr = expression();
            consume(TokenType::RPAREN, "Unclosed braces");
            return std::make_unique<Grouping>(std::move(expr));
        }
        default:
            throw ParseError("Expected expression", peek().pos);
    }
}

void Parser::synchronize() {
    advance();
    while (!isEOF())
    {
        if (getPrevToken().type == TokenType::SEMICOL) return;

        switch (peek().type) {
            KEYWORD_CASES
                return;
            default:
                break;
        }
        advance();
    }
}

std::vector<Parser::StmtNode> Parser::parse() {
    std::vector<Parser::StmtNode> ret;
    while (!match({TokenType::END}))
    {
        ret.push_back(declaration()); 
    }
    return ret;
}

Parser::Parser(std::vector<Token> &&toks) :  tokens(toks), cursor(0)  {  }
Parser::Parser() : cursor(0) {}
