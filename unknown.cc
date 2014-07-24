#include "vm.h"

#include "variable.h"

using namespace std;


Unknown::Unknown() : Variable(kTypeUnknown) {
}

Unknown::~Unknown() {
}

string Unknown::to_string() {
    return "";
}

Variable *Unknown::coerce_to_type(VariableType vt) {
    return NULL;
}

Variable *Unknown::add(Variable *) {
    return NULL;
}
Variable *Unknown::subtract(Variable *) {
    return NULL;
}
Variable *Unknown::multiply(Variable *) {
    return NULL;
}
Variable *Unknown::divide(Variable *) {
    return NULL;
}
Variable *Unknown::modulo(Variable *) {
    return NULL;
}

