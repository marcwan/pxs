#ifndef __REMOVEVAR_H_
#define __REMOVEVAR_H_

#include "instruction.h"


class RemoveVarInstruction : public Instruction {
  public:
    RemoveVarInstruction();
    ~RemoveVarInstruction();

    void execute(ScopeStack *);

  protected:
    void parse_and_validate_params();
};




#endif // __REMOVEVAR_H_
