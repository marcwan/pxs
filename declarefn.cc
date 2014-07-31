#include "vm.h"

#include "instruction.h"
#include "varpool.h"

using namespace std;


DeclareFnInstruction::DeclareFnInstruction() : Instruction() {
}

DeclareFnInstruction::~DeclareFnInstruction() {
}


bool DeclareFnInstruction::execute(IExecutionState *state, ScopeStack *scope_stack) {
    this->parse_and_validate_params();

    string fnname = this->m_args[0];
    Function *v = new Function(fnname, this->m_args[1]);

    Varpool *vp = scope_stack->get_current_scope();
    if (!vp)
        throw new InternalErrorException("This shouldn't be possible: No scope stack.");

    Variable *ev = vp->find_variable_by_name(fnname);
    if (ev) {
        v->release();
        ev->release();
        vp->release();
        throw new VariableAlreadyExistsException(fnname);
    }

    vp->set_value_for_name(fnname, v, false);

    vp->release();
    v->release(); // varpool takes ownership

    return false; // continue to next instruction
}



void DeclareFnInstruction::parse_and_validate_params() {
    if (this->m_args.size() != 2)
        throw new InternalErrorException("Expecting 2 args, got: %d", this->m_args.size());
    if (this->m_args[0].length() == 0)
        throw new InternalErrorException("Fn variable name must not be empty.");
    if (this->m_args[1].length() == 0)
        throw new InternalErrorException("Fn implementation name must not be empty.");
}
