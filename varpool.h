#ifndef __VARPOOL_H_
#define __VARPOOL_H_

#include "variable.h"


class Varpool : public Refcounted {
  public:
    Varpool();
    Varpool(const Varpool &copyme);
    virtual ~Varpool();

    virtual bool declare_var(std::string name, bool throw_if_exists = false);
    virtual bool set_value_for_var(std::string name,
                                   Variable *v,
                                   bool throw_if_not_exists = false);
    virtual bool set_value_for_name(std::string tmpname,
                                    Variable *v,
                                    bool throw_if_not_exists = false);

    virtual bool undeclare_var(std::string name, bool throw_if_not_exists = true);

    virtual Variable *find_variable_by_name(std::string var_name);

  protected:
    std::map<std::string, Variable *> m_vars;
};


struct VariableAlreadyExistsException : public std::exception {
    std::string varname;
    VariableAlreadyExistsException(std::string in) : varname(in) {}
    ~VariableAlreadyExistsException() throw () {} // Updated
    const char *what() const throw() {
        std::string s("Variable already declared: ");
        s += varname;
        return s.c_str();
    }
};

struct NoSuchVariableException : public std::exception {
    std::string varname;
    NoSuchVariableException(std::string in) : varname(in) {}
    ~NoSuchVariableException() throw () {} // Updated
    const char *what() const throw() {
        std::string s("Use of undeclared variable: ");
        s += varname;
        return s.c_str();
    }
};

struct NoSuchTempVariableException : public NoSuchVariableException {
    NoSuchTempVariableException(std::string in) : NoSuchVariableException(in) {}
};


#endif // __VARPOOL_H_
