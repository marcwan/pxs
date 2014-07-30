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




#endif //  __JUMP_H_
