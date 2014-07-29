#include "vm.h"

#include "constantparser.h"
#include "instruction.h"
#include <iomanip>
#include <sstream>
#include "varpool.h"

using namespace std;


/**
 * function decls
 */
byte _int_to_compare_state(int val);





CompareInstruction::CompareInstruction() {
}

CompareInstruction::~CompareInstruction() {
}



/**
 * basic format:  COMPARE var, var_or_const
 */
void CompareInstruction::execute(ScopeStack *scope_stack) {
    if (this->m_args.size() != 2)
        throw InternalErrorException("Unexpected # args for COMPARE");

    // this sets m_left and m_right
    this->parse_and_validate_params(scope_stack);

    byte result = this->compare();
    scope_stack->set_compare_flags(result);
}



void CompareInstruction::parse_and_validate_params(ScopeStack *scope_stack) {
    this->m_left = Instruction::get_const_or_var(scope_stack, this->m_args[0]);
    if (!this->m_left)
        throw UndeclaredVariableException(this->m_args[0]);
    this->m_right = Instruction::get_const_or_var(scope_stack, this->m_args[1]);
    if (!this->m_right)
        throw UndeclaredVariableException(this->m_args[1]);
}


byte CompareInstruction::compare() {
    VariableType vtl, vtr;
    vtl = this->m_left->get_type();
    vtr = this->m_right->get_type();
    if (vtl == vtr)
        return this->compare_equal_types();
    
    /**
     * Otherwise, let's just figure the rest of this out.
     */
    if (vtl == kTypeNaN || vtr == kTypeNaN)
        return kCompareAlwaysFalse;
    else if (vtl == kTypeUnknown || vtr == kTypeUnknown)
        return kCompareNotEqual;
    else if (vtl == kTypeInfinity || vtr == kTypeInfinity)
        return kCompareNotEqual;
    else if ((vtl == kTypeNumber && vtr == kTypeString)
             || (vtr == kTypeNumber && vtl == kTypeString)) 
        return this->compare_string_and_number();
    else if ((vtl == kTypeNumber && vtr == kTypeBoolean)
             || (vtr == kTypeNumber && vtl == kTypeBoolean)) 
        return this->compare_boolean_and_number();
    else if ((vtl == kTypeString && vtr == kTypeBoolean)
             || (vtr == kTypeString && vtl == kTypeBoolean)) 
        return this->compare_boolean_and_string();
    else {
        ostringstream str;
        str << "Unimplemented: Comparing vars of type ";
        str << vtl << " and " << vtr << "in " __FILE__ << endl;
        throw InternalErrorException(str.str().c_str());
    }
}



byte CompareInstruction::compare_equal_types() {
    switch (this->m_left->get_type()) {
        case kTypeNumber:
        {
            int val = Number::compare(this->m_left, this->m_right);
            if (val == 0)
                return kCompareIdentical | kCompareEqual;
            else
                return _int_to_compare_state(val);
        }
        case kTypeString:
        {
            int val = String::compare(this->m_left, this->m_right);
            if (val == 0)
                return kCompareIdentical | kCompareEqual;
            else
                return _int_to_compare_state(val);
        }
        case kTypeInfinity:
        {
            Infinity *l, *r;
            l = dynamic_cast<Infinity *>(this->m_left);
            r = dynamic_cast<Infinity *>(this->m_right);
            if (l->positive() == r->positive())
                return kCompareIdentical | kCompareEqual;
            else
                return kCompareNotEqual;
        }
        case kTypeBoolean:
        {
            Boolean *l, *r;
            l = dynamic_cast<Boolean *>(this->m_left);
            r = dynamic_cast<Boolean *>(this->m_right);
            if (l->get_value() == r->get_value())
                return kCompareIdentical | kCompareEqual;
            else
                return kCompareNotEqual;
        }
        case kTypeNaN:
            return kCompareAlwaysFalse;

        case kTypeUnknown:
            return kCompareIdentical | kCompareEqual;

        default:
            throw InternalErrorException("COMPARE NOT SUPPORTED YET");
    }
}


/**
 * If the string is entirely numeric, then we'll compare it
 * as a number, otherwise, not equal!
 */
byte CompareInstruction::compare_string_and_number() {
    Variable *num, *str;
    if (this->m_left->get_type() == kTypeNumber) {
        num = this->m_left;
        str = this->m_right;
    } else {
        num = this->m_right;
        str = this->m_left;
    }

    if (!String::is_numeric_string(str))
        return kCompareNotEqual;

    Variable *n = str->coerce_to_type(kTypeNumber);

    byte res;
    int val = Number::compare(num, n);
    res = _int_to_compare_state(val);
    
    n->release();
    return res;
}


byte CompareInstruction::compare_boolean_and_number() {
    Variable *num, *bul;
    if (this->m_left->get_type() == kTypeNumber) {
        num = this->m_left;
        bul = this->m_right;
    } else {
        num = this->m_right;
        bul = this->m_left;
    }

    bool b1, b2;
    b1 = dynamic_cast<Number *>(num)->get_value() != 0;
    b2 = dynamic_cast<Boolean *>(bul)->get_value();

    if (b1 == b2) 
        return kCompareEqual;
    else
        return kCompareNotEqual;
}



byte CompareInstruction::compare_boolean_and_string() {
    Variable *str, *bul;
    if (this->m_left->get_type() == kTypeString) {
        str = this->m_left;
        bul = this->m_right;
    } else {
        str = this->m_right;
        bul = this->m_left;
    }

    bool b1, b2;
    b1 = dynamic_cast<String *>(str)->get_value().size() > 0;
    b2 = dynamic_cast<Boolean *>(bul)->get_value();

    if (b1 == b2) 
        return kCompareEqual;
    else
        return kCompareNotEqual;
}






byte _int_to_compare_state(int val) {
    if (val == 0) {
        return kCompareEqual;
    } else if (val < 0) {
        return kCompareLessThan;
    } else {
        return kCompareGreaterThan;
    }
}
