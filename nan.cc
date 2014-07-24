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
    return new NaN();
}
Variable *NaN::subtract(Variable *) {
    return new NaN();
}
Variable *NaN::multiply(Variable *) {
    return new NaN();
}
Variable *NaN::divide(Variable *) {
    return new NaN();
}
Variable *NaN::modulo(Variable *) {
    return new NaN();
}

