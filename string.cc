#include "vm.h"

#include <ctype.h>
#include "variable.h"

using namespace std;

Variable *String::from_quoted_literal(string literal) {
    char f, l;

    // this shouldn't really happen.
    if (literal.length() < 2) return new String("");

    return new String(literal.substr(1, literal.size() - 2));
}



String::String() : Variable(kTypeString) {
}
String::String(string value) : Variable(kTypeString), m_value(value) {
}
String::String(const char *value) : Variable(kTypeString), m_value(value) {
}

String::~String() {
}

string String::to_string() {
    return this->m_value;
}

string String::get_value() {
    return this->m_value;
}


Variable *String::coerce_to_type(VariableType vt) {
    if (vt == kTypeString) {
        return new String(this->m_value);
    } else if (vt == kTypeNumber) {
        return this->convert_to_number();
    } else 
        throw InternalErrorException("Asked to coerce string to sth new.");
}


Variable *String::add(Variable *other) {
    Variable *v;
    if (String::is_numeric_string(this)
        && String::is_numeric_string(other)) {
        Variable *v1, *v2;
        v1 = this->coerce_to_type(kTypeNumber);
        v2 = other->coerce_to_type(kTypeNumber);
        Variable *retval = v1->add(v2);
        v1->release();
        v2->release();
        return retval;
    } else if (other->get_type() != kTypeString) {
        v = other->coerce_to_type(kTypeString);
    } else {
        v = other;
        v->addref();
    }

    return new String(this->m_value + dynamic_cast<String *>(v)->m_value);
    v->release();
}

Variable *String::subtract(Variable *other) {
    if (String::is_numeric_string(this)
        && String::is_numeric_string(other)) {
        Variable *v1, *v2;
        v1 = this->coerce_to_type(kTypeNumber);
        v2 = other->coerce_to_type(kTypeNumber);
        Variable *retval = v1->subtract(v2);
        v1->release();
        v2->release();
        return retval;
    } else 
        return new NaN();
}
Variable *String::multiply(Variable *other) {
    if (String::is_numeric_string(this)
        && String::is_numeric_string(other)) {
        Variable *v1, *v2;
        v1 = this->coerce_to_type(kTypeNumber);
        v2 = other->coerce_to_type(kTypeNumber);
        Variable *retval = v1->multiply(v2);
        v1->release();
        v2->release();
        return retval;
    } else 
        return new NaN();
}
Variable *String::divide(Variable *other) {
    if (String::is_numeric_string(this)
        && String::is_numeric_string(other)) {
        Variable *v1, *v2;
        v1 = this->coerce_to_type(kTypeNumber);
        v2 = other->coerce_to_type(kTypeNumber);
        Variable *retval = v1->divide(v2);
        v1->release();
        v2->release();
        return retval;
    } else 
        return new NaN();
    return new NaN();
}
Variable *String::modulo(Variable *other) {
    if (String::is_numeric_string(this)
        && String::is_numeric_string(other)) {
        Variable *v1, *v2;
        v1 = this->coerce_to_type(kTypeNumber);
        v2 = other->coerce_to_type(kTypeNumber);
        Variable *retval = v1->modulo(v2);
        v1->release();
        v2->release();
        return retval;
    } else 
        return new NaN();
}


Variable *String::convert_to_number() {
    if (isdigit(this->m_value[0])
        || (this->m_value[0] == '-' && isdigit(this->m_value[1]))) {
        return new Number(strtold(this->m_value.c_str(), NULL));
    } else {
        return new NaN();
    }
}


bool String::is_numeric_string(Variable *v) {
    if (v->get_type() != kTypeString) return false;

    String *s = dynamic_cast<String *>(v);
    string val = s->m_value;
    bool seen_period = false, seene = false;
    int i = 0;

    if (val[0] == '-') i++;

    while (i < val.length()) {
        if (val[i] == '.') {
            if (seen_period) return false;
            seen_period = true;
        } else if (tolower(val[i]) == 'e') {
            if (seene) return false;
            seene = true;
            if (i + 1 < val.length()
                && (val[i+1] == '-' || val[i+1] == '+')) {
                // cool.
                i++;
            }
        } else if (!isdigit(val[i]))
            return false;

        i++;
    }
    return true;
}
