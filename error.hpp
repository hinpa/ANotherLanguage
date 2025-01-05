#ifndef H_CUSTOM_EXCEPT_H
#define H_CUSTOM_EXCEPT_H
#include <stdexcept>
#include <string>

class IThrowError : std::exception { 
    std::string edesc;
    int pos_;
public:
    IThrowError() : pos_(-1) { }
    IThrowError(int pos, std::string desc) : pos_(pos), edesc(desc) { }
    IThrowError(const IThrowError& e) {
        edesc = e.edesc;
        pos_ = e.pos_;
    }
    IThrowError(IThrowError &&e) {
        edesc = std::move(e.edesc);
        pos_ = e.pos_;
    }
    IThrowError& operator=(IThrowError&& e) {
        edesc = std::move(e.edesc);
        pos_ = e.pos_;
        return *this; 
    }
    IThrowError& operator=(const IThrowError& e) {
        edesc = e.edesc;
        pos_ = e.pos_;
        return *this;
    }
    virtual const char *what() const noexcept override {
        return edesc.data();
    }
    int getPos() {
        return pos_;
    }
    ~IThrowError() {}
};



namespace PrettyError {
    extern const char *source;
    std::string prettifyError(IThrowError &e);
}
class NotImplemented : public std::runtime_error {
public:
    NotImplemented() : std::runtime_error("Not implemented") {}
    NotImplemented(const char *funcName, const char *fileName) : 
        std::runtime_error(std::string("Function ") + funcName + " not implemented in file: " + fileName) { }
};


class LexerError : public IThrowError {
public:
    LexerError(std::string s, size_t pos) : IThrowError(pos, s) { }
};

class ParseError : public IThrowError {
public:
    ParseError(std::string s, size_t pos) : IThrowError(pos, s) { }
};

class RuntimeError : public IThrowError {
public:
    RuntimeError(std::string s, size_t pos) : IThrowError(pos, s) { }
};

class OperatorNotImplemented : public std::runtime_error {
public:
    OperatorNotImplemented(const char *funcName, const char *operatorName)
        : std::runtime_error(std::string("Operator ") + operatorName 
                + " is not implemented(Reporting function: " + funcName + ")") { 
    }

};


#define NO_IMPL throw NotImplemented(__PRETTY_FUNCTION__,__FILE__); 
#define OP_NO_IMPL(x) throw OperatorNotImplemented(__PRETTY_FUNCTION__, x);
// Intermediate macros to force evaluation

enum ErrorCode {
    SUCCESS,
    FAILURE,
    NO_TOK,
    MALFORMED_LITERAL
};

struct Error {
    std::string description;
    ErrorCode error;
    Error() : error(ErrorCode::SUCCESS) { }
    Error(ErrorCode ec) : error(ec) { }
    Error(const std::string &s, ErrorCode ec) : description(s), error(ec) { }
    operator ErrorCode() {
        return error;
    }
};

template<typename T>
struct Maybe {
    T val;
    Error error;
    Maybe() {}
    Maybe(Error e) : error(e) { }
    Maybe(const T& val_, Error e) : val(val_), error(e) {}
    operator bool() {
        return error.error != ErrorCode::SUCCESS;
    }
    inline bool isErrorCodeSet() {
        return *this;
    }
};

#endif
