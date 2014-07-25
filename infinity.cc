#include "vm.h"

#include "variable.h"

using namespace std;

const char *kValueInfinity = "Infinity";
const char *kValueNegInfinity = "-Infinity";


Infinity::Infinity() : Variable(kTypeInfinity), m_positive(true) {
}

Infinity::Infinity(bool positive) : Variable(kTypeInfinity), m_positive(positive) {
}

Infinity::~Infinity() {
}

string Infinity::to_string() {
    if (this->m_positive)
        return kValueInfinity;
    else
        return kValueNegInfinity;
}

Variable *Infinity::coerce_to_type(VariableType vt) {
    if (vt == kTypeString) {
        if (this->m_positive)
            return new String(kValueInfinity);
        else
            return new String(kValueNegInfinity);
    } else if (vt == kTypeNumber) {
        return new Infinity();
    } else {
        throw InternalErrorException("Hum. Infinity coercsion to sth else");
    }
}

Variable *Infinity::add(Variable *) {
    return new Infinity();
}
Variable *Infinity::subtract(Variable *) {
    return new Infinity();
}
Variable *Infinity::multiply(Variable *) {
    return new Infinity();
}
Variable *Infinity::divide(Variable *) {
    return new Infinity();
}
Variable *Infinity::modulo(Variable *) {
    return new Infinity();
}

