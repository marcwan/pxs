#include "vm.h"

#include "variable.h"

using namespace std;


NaN::NaN() : Variable(kTypeNaN) {
}

NaN::~NaN() {
}

string NaN::to_string() {
    return "NaN";
}

Variable *NaN::coerce_to_type(VariableType vt) {
    if (vt == kTypeString) {
        return new String("NaN");
    } else if (vt == kTypeNumber) {
        return new NaN();
    } else {
        throw InternalErrorException("Hum. NaN coercsion to sth else");
    }
}

Variable *NaN::add(Variable *) {
    return NULL;
}
Variable *NaN::subtract(Variable *) {
    return NULL;
}
Variable *NaN::multiply(Variable *) {
    return NULL;
}
Variable *NaN::divide(Variable *) {
    return NULL;
}
Variable *NaN::modulo(Variable *) {
    return NULL;
}

