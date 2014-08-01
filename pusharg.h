#ifndef __PUSHARG_H_
#define __PUSHARG_H_


class PushArgInstruction : public Instruction {
  public:
    PushArgInstruction();
    ~PushArgInstruction();

    virtual bool execute(IExecutionState *, ScopeStack *);
};




#endif //  __PUSHARG_H_
