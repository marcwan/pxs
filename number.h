#ifndef __NUMBER_H_
#define __NUMBER_H_

#include "variable.h"

class Number : public Variable {
  public:
    Number();
    Number(int value);
    Number(long value);
    Number(long long value);
    Number(double value);
    Number(long double value);
    virtual ~Number();

    virtual void set_value(int value);
    virtual void set_value(long value);
    virtual void set_value(long long value);
    virtual void set_value(double value);
    virtual void set_value(long double value);

    virtual long double get_value();

    virtual Variable *coerce_to_type(VariableType);

    // all of these ops =  ME %op% arg1
    virtual Variable *add(Variable *);
    virtual Variable *subtract(Variable *);
    virtual Variable *multiply(Variable *);
    virtual Variable *divide(Variable *);
    virtual Variable *modulo(Variable *);

    virtual std::string to_string();

    static int compare(Variable *a, Variable *b);
    static int compare(Number *a, Number *b);
    
  protected:
    long double m_value;

    Variable *_perform_op(Variable *arg2,
                          long double(* popfn)(long double, long double));

};
    



#endif //  __NUMBER_H_
