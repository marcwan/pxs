#ifndef __STRING_H_
#define __STRING_H_

class String : public Variable {
  public:
    String();
    String(std::string);
    String(const char *);
    virtual ~String();

    virtual std::string to_string();

    virtual Variable *coerce_to_type(VariableType);

    virtual Variable *add(Variable *);
    virtual Variable *subtract(Variable *);
    virtual Variable *multiply(Variable *);
    virtual Variable *divide(Variable *);
    virtual Variable *modulo(Variable *);

    std::string get_value();

    static Variable *from_quoted_literal(std::string);
    static bool is_numeric_string(Variable *);

    static int compare(String *, String *);
    static int compare(Variable *, Variable *);

  protected:
    std::string m_value;

    Variable *convert_to_number();
};

#endif // __STRING_H_
