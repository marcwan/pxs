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
byte __int_to_compare_state(int val);





TestInstruction::TestInstruction(InstructionCode ic) : Instruction(), m_code(ic) {
}


TestInstruction::~TestInstruction() {
}



/**
 * basic format:  TEST var, var_or_const
 */
bool TestInstruction::execute(IExecutionState *state, ScopeStack *scope_stack) {
    if (this->m_args.size() != 3)
        throw new InternalErrorException("Unexpected # args for TEST");

    // this sets m_left and m_right
    this->parse_and_validate_params(scope_stack);

    bool return_value;
    byte result = this->compare();
    switch (this->m_code) {
        case kInstTESTEQ:
            return_value = (result & kCompareEqual) ? true : false;
            break;
        case kInstTESTNEQ:
            return_value = (result & kCompareNotEqual) ? true : false;
            break;
        case kInstTESTIDENT:
            return_value = (result & kCompareIdentical) ? true : false;
            break;
        case kInstTESTNIDENT:
            return_value = (result & kCompareIdentical) ? false : true;
            break;
        case kInstTESTGT:
            return_value = (result & kCompareGreaterThan) ? true : false;
            break;
        case kInstTESTGTE:
            if (result & kCompareGreaterThan || result & kCompareEqual)
                return_value = true;
            else
                return_value = false;
            break;
        case kInstTESTLT:
            return_value = (result & kCompareLessThan) ? true : false;
            break;
        case kInstTESTLTE:
            if (result & kCompareLessThan || result & kCompareEqual)
                return_value = true;
            else
                return_value = false;
            break;
        default:
            throw new InternalErrorException("Unsupported instruction in TEST");
    }

    // save out the result, and we're good to go!
    Variable *b = new Boolean(return_value);
    scope_stack->set_variable_in_scope(this->m_args[2], b);
    b->release();

    return false; // does NOT affect instruction pointer.
}



void TestInstruction::parse_and_validate_params(ScopeStack *scope_stack) {
    this->m_left = Instruction::get_const_or_var(scope_stack, this->m_args[0]);
    if (!this->m_left)
        throw new UndeclaredVariableException(this->m_args[0]);
    this->m_right = Instruction::get_const_or_var(scope_stack, this->m_args[1]);
    if (!this->m_right)
        throw new UndeclaredVariableException(this->m_args[1]);

    Variable *dest = scope_stack->find_variable_by_name(this->m_args[2]);
    if (!dest)
        throw new UndeclaredVariableException(this->m_args[2]);
    dest->release();
}



byte TestInstruction::compare() {
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
    else if (vtl == kTypeUndefined || vtr == kTypeUndefined)
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
        throw new InternalErrorException(str.str().c_str());
    }
}



byte TestInstruction::compare_equal_types() {
    switch (this->m_left->get_type()) {
        case kTypeNumber:
        {
            int val = Number::compare(this->m_left, this->m_right);
            if (val == 0)
                return kCompareIdentical | kCompareEqual;
            else
                return __int_to_compare_state(val);
        }
        case kTypeString:
        {
            int val = String::compare(this->m_left, this->m_right);
            if (val == 0)
                return kCompareIdentical | kCompareEqual;
            else
                return __int_to_compare_state(val);
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

        case kTypeUndefined:
            return kCompareIdentical | kCompareEqual;

        default:
            throw new InternalErrorException("TEST NOT SUPPORTED YET");
    }
}


/**
 * If the string is entirely numeric, then we'll compare it
 * as a number, otherwise, not equal!
 */
byte TestInstruction::compare_string_and_number() {
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
    res = __int_to_compare_state(val);
    
    n->release();
    return res;
}


byte TestInstruction::compare_boolean_and_number() {
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



byte TestInstruction::compare_boolean_and_string() {
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






byte __int_to_compare_state(int val) {
    if (val == 0) {
        return kCompareEqual;
    } else if (val < 0) {
        return kCompareLessThan;
    } else {
        return kCompareGreaterThan;
    }
}
