#ifndef __VARPOOL_H_
#define __VARPOOL_H_

#include <map>

#include "variable.h"


class Varpool : public Refcounted {
  public:
    Varpool();
    Varpool(const Varpool &copyme);
    virtual ~Varpool();

    virtual bool declare_var(std::string name);
    virtual std::string get_temp(VariableType type);
    virtual void set_value_for_var(std::string name, Variable *v);
    virtual void set_value_for_temp(std::string tmpname, Variable *v);

    virtual bool remove_tmp(std::string);
    virtual bool undeclare_var(std::string name, bool throw_on_failure = true);

    virtual Variable *find_variable_by_name(std::string var_name);

  protected:
    std::map<std::string, Variable *> m_vars;
    std::map<Variable *> m_tmps; 
};


struct VariableAlreadyExistsException : public std::exception {
    std::string varname;
    VariableAlreadyExistsException(std::string in) : varname(in) {}
    ~VariableAlreadyExistsException() throw () {} // Updated
    const char *what() const throw() { return varname.c_str(); }
};

struct NoSuchVariableException : public std::exception {
    std::string varname;
    NoSuchVariableException(std::string in) : varname(in) {}
    ~NoSuchVariableException() throw () {} // Updated
    const char *what() const throw() { return varname.c_str(); }
};

struct NoSuchTempVariableException : public NoSuchVariableException {
    NoSuchTempVariableException() : NoSuchVariableException("temp var") {}
};


#endif // __VARPOOL_H_
