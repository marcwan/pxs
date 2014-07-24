#ifndef __GETTEMP_H_
#define __GETTEMP_H_

#include "instruction.h"


class GetTempInstruction : public Instruction {
  public:
    GetTempInstruction();
    ~GetTempInstruction();

    void execute(ScopeStack *);
};




#endif // __GETTEMP_H_
