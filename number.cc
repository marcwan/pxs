#include "vm.h"

#include "math.h"
#include "number.h"

using namespace std;



Number::Number() : Variable(kTypeNumber), m_value(0) {
   
}
Number::Number(int value) : Variable(kTypeNumber) {
    m_value = (long double)value;
}
Number::Number(long value) : Variable(kTypeNumber) {
    m_value = (long double)value;
}
Number::Number(long long value) : Variable(kTypeNumber) {
    m_value = (long double)value;
}
Number::Number(double value) : Variable(kTypeNumber) {
    m_value = (long double)value;
}
Number::Number(long double value) : Variable(kTypeNumber) {
    m_value = (long double)value;
}
Number::~Number() {
}


string Number::to_string() {
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "%Lf", m_value);

    // UNDONE: trim trailing zeros!!!!
    return string(buffer);
}


void Number::set_value(int value) {
    m_value = (long double)value;
}
void Number::set_value(long value) {
    m_value = (long double)value;
}
void Number::set_value(long long value) {
    m_value = (long double)value;
}
void Number::set_value(double value) {
    m_value = (long double)value;
}
void Number::set_value(long double value) {
    m_value = (long double)value;
}
long double Number::get_value() {
    return m_value;
}

Variable *Number::coerce_to_type(VariableType vt) {
    switch (vt) {
        case kTypeNumber: return new Number(this->m_value);
        case kTypeString: return new String(this->to_string());
        default: throw InternalErrorException("NOT IMPLEMENTED YET");
    }
}


Variable *Number::add(Variable *n) {
    Variable *v;
    if (n->get_type() == kTypeNaN)
        return new NaN();
    else if (n->get_type() != kTypeNumber) {
        v = n->coerce_to_type(kTypeNumber);
    } else {
        v = n;
        v->addref();
    }

    Number *num = new Number(this->m_value + dynamic_cast<Number *>(v)->get_value());
    v->release();
    return num;
}

Variable *Number::subtract(Variable *n) {
    Variable *v;
    if (n->get_type() == kTypeNaN)
        return new NaN();
    else if (n->get_type() != kTypeNumber) {
        v = n->coerce_to_type(kTypeNumber);
    } else {
        v = n;
        v->addref();
    }

    Number *num = new Number(this->m_value - dynamic_cast<Number *>(v)->get_value());
    v->release();
    return num;
}
Variable *Number::multiply(Variable *n) { 
    Variable *v;
    if (n->get_type() == kTypeNaN)
        return new NaN();
    else if (n->get_type() != kTypeNumber) {
        v = n->coerce_to_type(kTypeNumber);
    } else {
        v = n;
        v->addref();
    }

    Number *num = new Number(this->m_value * dynamic_cast<Number *>(v)->get_value());
    v->release();
    return num;
}
Variable *Number::divide(Variable *n) {
    Variable *v;
    if (n->get_type() == kTypeNaN)
        return new NaN();
    else if (n->get_type() != kTypeNumber) {
        v = n->coerce_to_type(kTypeNumber);
    } else {
        v = n;
        v->addref();
    }

    Number *num = new Number(this->m_value / dynamic_cast<Number *>(n)->get_value());
    v->release();
    return num;
}
Variable *Number::modulo(Variable *n) {
    Variable *v;
    if (n->get_type() == kTypeNaN)
        return new NaN();
    else if (n->get_type() != kTypeNumber) {
        v = n->coerce_to_type(kTypeNumber);
    } else {
        v = n;
        v->addref();
    }

    Number *num = new Number(fmodl(this->m_value, dynamic_cast<Number *>(n)->get_value()));
    v->release();
    return num;
}



int Number::compare(Variable *a, Variable *b) {
    Number *n1, *n2;

    if (a->get_type() != kTypeNumber
        || a->get_type() != kTypeNumber)
        throw InternalErrorException("Number compare is only for numbers");

    n1 = dynamic_cast<Number *>(a);
    n2 = dynamic_cast<Number *>(b);

    return Number::compare(n1, n2);
}


int Number::compare(Number *a, Number *b) {
    long double val = a->get_value() - b->get_value();
    if (val == 0)
        return 0;
    else if (val < 0)
        return -1;
    else
        return 1;
}



/*
i don't think i can use these, because i'll want different semantics
for differnet types.  i'll leave it here in case!

long double _add(long double op1, long double op2);
long double _sub(long double op1, long double op2);
long double _mul(long double op1, long double op2);
long double _div(long double op1, long double op2);
long double _mod(long double op1, long double op2);




long double _add(long double op1, long double op2) {
    return op1 + op2;
}
long double _sub(long double op1, long double op2) {
    return op1 - op2;
}
long double _mul(long double op1, long double op2) {
    return op1 * op2;
}
long double _div(long double op1, long double op2) {
    return op1 / op2;
}
long double _mod(long double op1, long double op2) {
    return op1 % op2;
}
*/
