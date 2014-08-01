#include "vm.h"

#include "variable.h"

using namespace std;

const char *kValueUndefined = "undefined";



Undefined::Undefined() : Variable(kTypeUndefined) {
}

Undefined::~Undefined() {
}

string Undefined::to_string() {
    return kValueUndefined;
}

Variable *Undefined::coerce_to_type(VariableType vt) {
    switch (vt) {
        case kTypeString:
            return new String(this->to_string());
        case kTypeUndefined:
            return new Undefined();
        case kTypeBoolean:
            return new Boolean(false);
        case kTypeNaN:
        case kTypeNumber:
            return new NaN();
        default:
            throw new InternalErrorException("I don't support converting undefined to this type.");
    }
}

Variable *Undefined::add(Variable *) {
    return new NaN();
}
Variable *Undefined::subtract(Variable *) {
    return new NaN();
}
Variable *Undefined::multiply(Variable *) {
    return new NaN();
}
Variable *Undefined::divide(Variable *) {
    return new NaN();
}
Variable *Undefined::modulo(Variable *) {
    return new NaN();
}

