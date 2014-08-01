#ifndef __POPARG_H_
#define __POPARG_H_


class PopArgInstruction : public Instruction {
  public:
    PopArgInstruction();
    ~PopArgInstruction();

    virtual bool execute(IExecutionState *, ScopeStack *);
};




#endif //  __POPARG_H_
