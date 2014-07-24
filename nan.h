#ifndef __NAN_H_
#define __NAN_H_

class NaN : public Variable {
  public:
    NaN();
    virtual ~NaN();

    virtual std::string to_string();

    virtual Variable *coerce_to_type(VariableType);

    virtual Variable *add(Variable *);
    virtual Variable *subtract(Variable *);
    virtual Variable *multiply(Variable *);
    virtual Variable *divide(Variable *);
    virtual Variable *modulo(Variable *);

};

#endif // __NAN_H_
