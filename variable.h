#ifndef __VARIABLE_H_
#define __VARIABLE_H_

#include "refcounted.h"

extern const char *kTypeNameNumber;
extern const char *kTypeNameString;
extern const char *kTypeNameBoolean;
extern const char *kTypeNameArray;
extern const char *kTypeNameObject;
extern const char *kTypeNameFunction;
extern const char *kTypeNameUndefined;
extern const char *kTypeNameNaN;
extern const char *kTypeNameInfinity;


typedef enum VariableType {
    kTypeNumber,
    kTypeString,
    kTypeBoolean,
    kTypeArray,
    kTypeObject,
    kTypeFunction,
    kTypeUndefined,
    kTypeNaN,
    kTypeInfinity
} VariableType;



class Variable : public Refcounted {
  public:
    static Variable *create_for_type(VariableType t);

    Variable(VariableType type);
    virtual ~Variable();

    inline VariableType get_type() { return m_type; };

    virtual std::string to_string() = 0;

    virtual Variable *coerce_to_type(VariableType) = 0;

    virtual Variable *add(Variable *) = 0;
    virtual Variable *subtract(Variable *) = 0;
    virtual Variable *multiply(Variable *) = 0;
    virtual Variable *divide(Variable *) = 0;
    virtual Variable *modulo(Variable *) = 0;

  protected:
    VariableType m_type;
};



VariableType string_to_variable_type(std::string);
std::string variable_type_to_string(VariableType);



struct InvalidVariableTypeException : public std::exception {
    std::string message;
    InvalidVariableTypeException(std::string in) : message(in) {}
    ~InvalidVariableTypeException() throw () {} // Updated
    const char *what() const throw() {
        std::string s("Cannot convert to type: ");
        s += message;
        return s.c_str();
    }
};


struct UndeclaredVariableException : public std::exception {
    std::string message;
    UndeclaredVariableException(std::string in) : message(in) {}
    ~UndeclaredVariableException() throw () {} // Updated
    const char *what() const throw() {
        std::string s("Undeclared variable: ");
        s += message;
        return s.c_str();
    }
};



struct OperationForTypeNotSupported : public std::exception {
    std::string message;
    OperationForTypeNotSupported(std::string op,
                                 std::string mytype,
                                 VariableType other_type);
    ~OperationForTypeNotSupported() throw () {}
    // this has a full cons in variable.cc
    const char *what() const throw() { return message.c_str(); }
};


#include "boolean.h"
#include "infinity.h"
#include "function.h"
#include "number.h"
#include "nan.h"
#include "string.h"
#include "undefined.h"

#endif // __VARIABLE_H_
