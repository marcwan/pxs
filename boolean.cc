#include "vm.h"

#include "variable.h"

using namespace std;

const char *kValueTrue = "true";
const char *kValueFalse = "false";


Boolean::Boolean() : Variable(kTypeBoolean), m_value(false) {
}

Boolean::Boolean(bool value) : Variable(kTypeBoolean), m_value(value) {
}

Boolean::~Boolean() {
}

bool Boolean::get_value() {
    return this->m_value;
}

string Boolean::to_string() {
    if (this->m_value)
        return kValueTrue;
    else
        return kValueFalse;
}


Variable *Boolean::coerce_to_type(VariableType vt) {
    if (vt == kTypeNumber)
        return new Number(this->m_value ? 1 : 0);
    else if (vt == kTypeString)
        return new String(this->to_string());
    else {
        ostringstream str;
        str << "Unimplemented: Convert bool to " << vt << endl;
        throw InternalErrorException(str.str().c_str());
    }
}

Variable *Boolean::add(Variable *) {
    throw InternalErrorException("UNIMPLEMENTED: Boolean::add");
}
Variable *Boolean::subtract(Variable *) {
    throw InternalErrorException("UNIMPLEMENTED: Boolean::subtract");
}
Variable *Boolean::multiply(Variable *) {
    throw InternalErrorException("UNIMPLEMENTED: Boolean::multiply");
}
Variable *Boolean::divide(Variable *) {
    throw InternalErrorException("UNIMPLEMENTED: Boolean::divide");
}
Variable *Boolean::modulo(Variable *) {
    throw InternalErrorException("UNIMPLEMENTED: Boolean::modulo");
}

