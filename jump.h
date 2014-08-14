#ifndef __JUMP_H_
#define __JUMP_H_


class JumpInstruction : public Instruction {
  public:
    JumpInstruction(InstructionCode);
    ~JumpInstruction();

    virtual bool execute(IExecutionState *, ScopeStack *);

  protected:
    InstructionCode m_op;
};



struct ExpectedBooleanException : public std::exception {
    std::string message;
    VariableType vt;
     ExpectedBooleanException(std::string in, VariableType typ) : message(in), vt(typ) {}
    ~ExpectedBooleanException() throw () {} // Updated
    const char *what() const throw() {
        char buf[50];
        snprintf(buf, sizeof(buf), "%d", vt);
        std::string s("Value for: ");
        s += message;
        s += std::string(" (") + std::string(buf)
            + std::string(") does not evaluate to a boolean.");
        return s.c_str();
    }
};


#endif //  __JUMP_H_
