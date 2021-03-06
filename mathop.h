#ifndef __MATHOP_H_
#define __MATHOP_H_


class MathopInstruction : public Instruction {
  public:
    MathopInstruction(InstructionCode);
    ~MathopInstruction();

    virtual bool execute(IExecutionState *, ScopeStack *);

  protected:
    InstructionCode m_code;
    void parse_and_validate_params(ScopeStack *);

    Variable *m_left, *m_right;
};




#endif //  __MATHOP_H_
