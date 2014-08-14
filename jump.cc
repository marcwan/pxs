#include "vm.h"

#include "constantparser.h"
#include "instruction.h"
#include "varpool.h"

using namespace std;


JumpInstruction::JumpInstruction(InstructionCode type) : Instruction(), m_op(type) {
}

JumpInstruction::~JumpInstruction() {
}



/**
 * m_args[0] is the label to which we'll jump. this is never going to
 * be a user error (it'd be a compiler bug) if it's wrong, so we'll
 * just throw if it's bad.
 */
bool JumpInstruction::execute(IExecutionState *state, ScopeStack *scope_stack) {
    string label;

    if (this->m_op == kInstJUMPF || this->m_op == kInstJUMPT) {
        if (this->m_args.size() != 2)
            throw new InternalErrorException("Wrong # of args for JUMPF/T.");
        Variable *v = Instruction::get_const_or_var(scope_stack, this->m_args[0]);

        if (!v)
            throw new UndeclaredVariableException(this->m_args[0]);
        else if (v->get_type() != kTypeBoolean)
            throw new ExpectedBooleanException(this->m_args[0], v->get_type());

        Boolean *b = dynamic_cast<Boolean *>(v);
        bool val = b->get_value();
        b->release();
        label = this->m_args[1];
        if (this->m_op == kInstJUMPF && !val) {
            state->jump_to_label(label);
            return true;
        } else if (this->m_op == kInstJUMPT && val) {
            state->jump_to_label(label);
            return true;
        } else {
            return false;
        }
    } else {
        if (this->m_args.size() != 1)
            throw new InternalErrorException("Wrong # of args for JUMPXYZ");
        label = this->m_args[0];
            state->jump_to_label(label); 
            return true;
    }
/*
    switch (this->m_op) {
        case kInstJUMP:
        case kInstJUMPEQ:
            if (flags & kCompareEqual) {
                state->jump_to_label(label);
                return true;
            }
            break;
        case kInstJUMPIDENT:
            if (flags & kCompareIdentical) {
                state->jump_to_label(label);
                return true;
            }
            break;
        case kInstJUMPNEQ:
            if (!(flags & kCompareEqual)) {
                state->jump_to_label(label);
                return true;
            }
            break;
        case kInstJUMPNIDENT:
            if (!(flags & kCompareIdentical)) {
                state->jump_to_label(label);
                return true;
            }
            break;
        case kInstJUMPGT:
            if (flags & kCompareGreaterThan) {
                state->jump_to_label(label);
                return true;
            }
            break;
        case kInstJUMPGTE:
            if ((flags & kCompareGreaterThan) || (flags & kCompareEqual)) {
                state->jump_to_label(label);
                return true;
            }
            break;
        case kInstJUMPLT:
            if (flags & kCompareLessThan) {
                state->jump_to_label(label);
                return true;
            }
            break;
        case kInstJUMPLTE:
            if ((flags & kCompareLessThan) || (flags & kCompareEqual)) {
                state->jump_to_label(label);
                return true;
            }
            break;
        default:
            throw new InternalErrorException("Are you sure that's a jump?");
    }
*/
    // didn't actually modify instruction pointer. as you were.
    return false;
}

