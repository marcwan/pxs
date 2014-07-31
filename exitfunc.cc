#include "vm.h"

#include "instruction.h"
#include "varpool.h"

using namespace std;


ExitFuncInstruction::ExitFuncInstruction() : Instruction() {
}


ExitFuncInstruction::~ExitFuncInstruction() {
}


bool ExitFuncInstruction::execute(IExecutionState *state, ScopeStack *scope_stack) {
    this->m_retval = Instruction::get_const_or_var(scope_stack, this->m_args[0]);
    state->exit_function(this->m_retval);

    return false; // we didn't modify instruction stream -- continue to next
}


