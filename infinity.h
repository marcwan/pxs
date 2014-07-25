#ifndef __INFINITY_H_
#define __INFINITY_H_


extern const char *kValueInfinity;
extern const char *kValueNegInfinity;


class Infinity : public Variable {
  public:
    Infinity();
    Infinity(bool positive);
    virtual ~Infinity();

    virtual std::string to_string();

    virtual Variable *coerce_to_type(VariableType);

    virtual Variable *add(Variable *);
    virtual Variable *subtract(Variable *);
    virtual Variable *multiply(Variable *);
    virtual Variable *divide(Variable *);
    virtual Variable *modulo(Variable *);
  protected:
    bool m_positive;
};

#endif // __INFINITY_H_
