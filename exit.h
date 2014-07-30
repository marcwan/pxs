#ifndef __EXIT_H_
#define __EXIT_H_


class ExitInstruction : public Instruction {
  public:
    ExitInstruction();
    ~ExitInstruction();

    virtual void execute(IExecutionState *, ScopeStack *);

  protected:
    Variable *m_exitcode;
};




#endif //  __EXIT_H_
