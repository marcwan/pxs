#ifndef __COMPARE_H_
#define __COMPARE_H_


class CompareInstruction : public Instruction {
  public:
    CompareInstruction();
    ~CompareInstruction();

    /** 
     * compare teh two operands and put the resulting state in the 
     * scope stack!
     */
    void execute(ScopeStack *);

  protected:
    Variable *m_left, *m_right;

    void parse_and_validate_params(ScopeStack *);
    Variable *extract_const_or_variable(ScopeStack *, std::string);

    byte compare();

    byte compare_equal_types();
    byte compare_string_and_number();
    byte compare_boolean_and_number();
    byte compare_boolean_and_string();
};




#endif //  __COMPARE_H_
