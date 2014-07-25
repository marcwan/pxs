#ifndef __SET_H_
#define __SET_H_


class SetInstruction : public Instruction {
  public:
    SetInstruction();
    ~SetInstruction();

    void execute(ScopeStack *);
};




#endif //  __SET_H_
