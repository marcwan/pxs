#include "vm.h"

#include "variable.h"

using namespace std;



Function::Function(string impl) : Variable(kTypeFunction),
                                  m_name(""),
                                  m_implementation(impl) {
    if (this->m_implementation.length() == 0)
        throw new InternalErrorException("Function needs an implementation");
}

Function::Function(string name, string impl) : Variable(kTypeFunction),
                                               m_name(name),
                                               m_implementation(impl) {
    if (this->m_implementation.length() == 0)
        throw new InternalErrorException("Function needs an implementation");
}

Function::~Function() {
}

string Function::to_string() {
    char buf[this->m_name.length() + 32];
    snprintf(buf, sizeof(buf), "function %s (...) { ... }", this->m_name.c_str());
    return buf;
}

Variable *Function::coerce_to_type(VariableType vt) {
    if (vt == kTypeString) {
        return new String(this->to_string());
    } else if (vt == kTypeNumber) {
        return new NaN();
    } else {
        throw new InternalErrorException("Hum. Function coercsion to sth else");
    }
}

Variable *Function::add(Variable *) {
    return new NaN();
}
Variable *Function::subtract(Variable *) {
    return new NaN();
}
Variable *Function::multiply(Variable *) {
    return new NaN();
}
Variable *Function::divide(Variable *) {
    return new NaN();
}
Variable *Function::modulo(Variable *) {
    return new NaN();
}

