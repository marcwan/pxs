#ifndef __ADD_H_
#define __ADD_H_


class AddInstruction : public Instruction {
  public:
    AddInstruction();
    ~AddInstruction();

    void execute(ScopeStack *);


  protected:
    void parse_and_validate_params(ScopeStack *);

    Variable *m_left, *m_right, *m_destination;

};




#endif //  __ADD_H_
