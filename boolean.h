#ifndef __BOOLEAN_H_
#define __BOOLEAN_H_


extern const char *kValueTrue;
extern const char *kValueFalse;

class Boolean : public Variable {
  public:
    Boolean();
    Boolean(bool value);
    virtual ~Boolean();

    virtual std::string to_string();

    virtual Variable *coerce_to_type(VariableType);

    virtual Variable *add(Variable *);
    virtual Variable *subtract(Variable *);
    virtual Variable *multiply(Variable *);
    virtual Variable *divide(Variable *);
    virtual Variable *modulo(Variable *);

    virtual bool get_value();

  protected:
    bool m_value;

};

#endif // __BOOLEAN_H_
