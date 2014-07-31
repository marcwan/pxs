#ifndef __DECLAREFN_H_
#define __DECLAREFN_H_

#include "instruction.h"


class DeclareFnInstruction : public Instruction {
  public:
    DeclareFnInstruction();
    ~DeclareFnInstruction();

    virtual bool execute(IExecutionState *, ScopeStack *);

  protected:
    void parse_and_validate_params();
};




#endif // __DECLAREFN_H_
