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
    if (this->m_args.size() != 1)
        throw new InternalErrorException("Need a destination label for JUMPXYZ inst.");

    string label = this->m_args[0];
    byte flags = state->get_compare_flags();

    switch (this->m_op) {
        case kInstJUMP:
            state->jump_to_label(label); 
            return true;
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

    // didn't actually modify instruction pointer. as you were.
    return false;
}

