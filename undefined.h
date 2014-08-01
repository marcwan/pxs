#ifndef __UNDEFINED_H_
#define __UNDEFINED_H_

extern const char *kValueUndefined;

class Undefined : public Variable {
  public:
    Undefined();
    virtual ~Undefined();

    virtual std::string to_string();

    virtual Variable *coerce_to_type(VariableType);

    virtual Variable *add(Variable *);
    virtual Variable *subtract(Variable *);
    virtual Variable *multiply(Variable *);
    virtual Variable *divide(Variable *);
    virtual Variable *modulo(Variable *);

};

#endif // __UNDEFINED_H_
