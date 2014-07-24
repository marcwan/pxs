#ifndef __EXIT_H_
#define __EXIT_H_


class ExitInstruction : public Instruction {
  public:
    ExitInstruction();
    ~ExitInstruction();

    void execute(ScopeStack *);
};




#endif //  __EXIT_H_
