#include "vm.h"

#include "instruction.h"
#include "varpool.h"

using namespace std;


RemoveVarInstruction::RemoveVarInstruction() : Instruction() {
}

RemoveVarInstruction::~RemoveVarInstruction() {
}


bool RemoveVarInstruction::execute(IExecutionState *state, ScopeStack *scope_stack) {
    this->parse_and_validate_params();

    Varpool *vp = scope_stack->get_current_scope();
    // remove it and throw if it's not there.
    vp->undeclare_var(this->m_args[0], true);
    vp->release();
    return false;
}


void RemoveVarInstruction::parse_and_validate_params() {
    if (this->m_args.size() != 1)
        throw InternalErrorException("Expecting 1 arg, got: %d", this->m_args.size());

    if (this->m_args[0].length() == 0)
        throw InternalErrorException("Variable name must not be empty.");
}
