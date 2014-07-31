#ifndef __FUNCTION_H_
#define __FUNCTION_H_


class Function : public Variable {
  public:
    Function();
    Function(bool positive);
    virtual ~Function();

    virtual std::string to_string();

    virtual Variable *coerce_to_type(VariableType);

    inline bool positive() { return this->m_positive; }

    virtual Variable *add(Variable *);
    virtual Variable *subtract(Variable *);
    virtual Variable *multiply(Variable *);
    virtual Variable *divide(Variable *);
    virtual Variable *modulo(Variable *);
  protected:
    bool m_positive;
    std::string m_implementation;  // name in fn table. mandatory!
    std::string m_name;            // can be ""
};


#endif // __FUNCTION_H_
