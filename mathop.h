#ifndef __MATHOP_H_
#define __MATHOP_H_


class MathopInstruction : public Instruction {
  public:
    MathopInstruction(InstructionCode);
    ~MathopInstruction();

    void execute(ScopeStack *);


  protected:
    InstructionCode m_code;
    void parse_and_validate_params(ScopeStack *);
    Variable *get_variable_for_value(ScopeStack *, std::string);

    Variable *m_left, *m_right;
};




#endif //  __MATHOP_H_
