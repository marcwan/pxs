#ifndef __TEST_H_
#define __TEST_H_


class TestInstruction : public Instruction {
  public:
    TestInstruction(InstructionCode);
    ~TestInstruction();

    /** 
     * test teh two operands and put the resulting state in the 
     * scope stack!
     */
    virtual bool execute(IExecutionState *, ScopeStack *);

  protected:
    InstructionCode m_code;
    Variable *m_left, *m_right;

    void parse_and_validate_params(ScopeStack *);
    Variable *extract_const_or_variable(ScopeStack *, std::string);

    byte compare();

    byte compare_equal_types();
    byte compare_string_and_number();
    byte compare_boolean_and_number();
    byte compare_boolean_and_string();
};




#endif //  __TEST_H_
