#include <any>
#include <cstdlib>
#include <initializer_list>
#include <iostream>
#include <fstream>
#include "error.hpp"
#include "interpreter.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "rulegen/ast.hpp"

class AstPrinter : public IExprVisitor {
    std::string s;
    void paren(std::string name, std::initializer_list<Expression *> expressions) {
        s.append("(").append(name);
        for (auto e : expressions) {
            s.append(" ");
            e->accept(this);
        }
        s.append(")");
    }
public:
    AstPrinter(Expression *expr) { print(expr); }
private:
    void print(Expression *expr) {
        expr->accept(this);
        std::cout << s;
    }
    virtual ExpressionResult visitBinary(Binary& b) { 
        paren(b.tok.lexeme, {b.left.get(), b.right.get() });
        return 0.;
    }
    virtual ExpressionResult visitGrouping(Grouping& g) {
        paren("group", {g.expr.get()});
        return 0.;
    }
    virtual ExpressionResult visitLiteral(Literal& l) { 
        s.append(l.tok.lexeme);
        return 0.;
    }
    virtual ExpressionResult visitUnary(Unary& u) { 
        paren(u.tok.lexeme, {u.expr.get()});
        return 0.;
    }
};

void printTokens(const Lexer::container& cont) {
    for (const auto &tok : cont) {
        std::cout << "----\n";
        std::cout << "Lexeme:" << tok.lexeme << std::endl;
        std::cout << "position:" << tok.pos << std::endl;
        std::cout << "Type:" << tokenNames[(size_t)tok.type] << std::endl;
    }
}


int main(int argc, const char **argv) {
    if (argc < 2) {
        std::cout << "Usage: ./holy <file>" << std::endl;
        return EXIT_FAILURE;
    }

    std::ifstream of(argv[1]);
    char buffer[1024];
    
    std::string source;
    while (of.read(buffer, 1024)) {
        source += buffer;
    }
    buffer[of.gcount()] = '\0';
    source += buffer;

    PrettyError::source = source.data();


    try { 
        Lexer lexer(source);
        lexer.scanSource();

        Parser parser(lexer.getTokens());

        std::vector<Parser::StmtNode> e = parser.parse();

//       (AstPrinter(e.get()));
        Interpreter i;
        i.run(e);
    } catch (IThrowError e) {
        std::cout << PrettyError::prettifyError(e) << std::endl;
    }
    return EXIT_SUCCESS;
}
