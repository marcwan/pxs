#ifndef __UNKNOWN_H_
#define __UNKNOWN_H_

class Unknown : public Variable {
  public:
    Unknown();
    virtual ~Unknown();

    virtual std::string to_string();

    virtual Variable *coerce_to_type(VariableType);

    virtual Variable *add(Variable *);
    virtual Variable *subtract(Variable *);
    virtual Variable *multiply(Variable *);
    virtual Variable *divide(Variable *);


};

#endif // __UNKNOWN_H_
