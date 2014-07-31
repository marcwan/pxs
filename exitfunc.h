#ifndef __EXITFUNC_H_
#define __EXITFUNC_H_

#include "instruction.h"


class ExitFuncInstruction : public Instruction {
  public:
    ExitFuncInstruction();
    ~ExitFuncInstruction();

    virtual bool execute(IExecutionState *, ScopeStack *);

  protected:
    Variable *m_retval;
};




#endif // __EXITFUNC_H_
