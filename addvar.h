#ifndef __ADDVAR_H_
#define __ADDVAR_H_

#include "instruction.h"


class AddVarInstruction : public Instruction {
  public:
    AddVarInstruction();
    ~AddVarInstruction();

    virtual bool execute(IExecutionState *, ScopeStack *);

  protected:
    void parse_and_validate_params();
};




#endif // __ADDVAR_H_
