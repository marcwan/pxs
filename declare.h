#ifndef __DECLARE_H_
#define __DECLARE_H_

#include "instruction.h"


class DeclareInstruction : public Instruction {
  public:
    DeclareInstruction();
    ~DeclareInstruction();

    virtual bool execute(IExecutionState *, ScopeStack *);

  protected:
    void parse_and_validate_params();
};




#endif // __DECLARE_H_
