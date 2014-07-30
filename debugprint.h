#ifndef __DEBUGPRINT_H_
#define __DEBUGPRINT_H_

extern const char *kNameDebugFlags;



class DebugPrintInstruction : public Instruction {
  public:
    DebugPrintInstruction();
    ~DebugPrintInstruction();

    virtual bool execute(IExecutionState *, ScopeStack *);
};




#endif //  __DEBUGPRINT_H_
