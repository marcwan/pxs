#ifndef __CALLFUNC_H_
#define __CALLFUNC_H_

#include "instruction.h"


class CallFuncInstruction : public Instruction {
  public:
    CallFuncInstruction();
    ~CallFuncInstruction();

    virtual bool execute(IExecutionState *, ScopeStack *);

  protected:
    void parse_and_validate_params();
};



struct NoSuchFunctionException : public std::exception {
    std::string message;
    NoSuchFunctionException(std::string in) : message(in) {}
    ~NoSuchFunctionException() throw () {} // Updated
    const char *what() const throw() {
        std::string s("No function named: ");
        s += message;
        return s.c_str();
    }
};

struct NotAFunctionException : public std::exception {
    std::string message;
    NotAFunctionException(std::string in) : message(in) {}
    ~NotAFunctionException() throw () {} // Updated
    const char *what() const throw() {
        std::string s("The variable '");
        s += message;
        s += "' is not a function.";
        return s.c_str();
    }
};


#endif // __CALLFUNC_H_
