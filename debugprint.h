#ifndef __DEBUGPRINT_H_
#define __DEBUGPRINT_H_


class DebugPrintInstruction : public Instruction {
  public:
    DebugPrintInstruction();
    ~DebugPrintInstruction();

    void execute(ScopeStack *);
};




#endif //  __DEBUGPRINT_H_
