#include "vm.h"

#include "instruction.h"
#include "varpool.h"

using namespace std;


AddVarInstruction::AddVarInstruction() : Instruction() {
}

AddVarInstruction::~AddVarInstruction() {
}

void AddVarInstruction::execute(ScopeStack *scope_stack) {
    Variable *v = scope_stack->find_variable_by_name(this->m_args[0]);
    // only add this var if we don't have it in this scope. we don't care
    // if they re-declare it in this scope.
    if (!v) {
        Varpool *vp = scope_stack->get_current_scope();
        vp->declare_var(this->m_args[0]);
        vp->release();
    } else {
        // UNDONE: add a runtime flag to be strict?
        v->release();
    }
}


void AddVarInstruction::parse_and_validate_params() {
    if (this->m_args.size() != 1)
        throw InternalErrorException("Expecting 1 arg, got: %d", this->m_args.size());

    if (this->m_args[0].length() == 0)
        throw InternalErrorException("Variable name must not be empty.");
}
