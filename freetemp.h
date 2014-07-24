#ifndef __FREETEMP_H_
#define __FREETEMP_H_

#include "instruction.h"


class FreeTempInstruction : public Instruction {
  public:
    FreeTempInstruction();
    ~FreeTempInstruction();

    void execute(ScopeStack *);
};




#endif // __FREETEMP_H_
