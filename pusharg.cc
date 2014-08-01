
#include "vm.h"

#include "constantparser.h"
#include "instruction.h"
#include "varpool.h"

using namespace std;


PushArgInstruction::PushArgInstruction() : Instruction() {
}

PushArgInstruction::~PushArgInstruction() {
}



/**
 * basic format:  PUSHARG var_or_const
 */
bool PushArgInstruction::execute(IExecutionState *state, ScopeStack *scope_stack) {
    Variable *v = Instruction::get_const_or_var(scope_stack, this->m_args[0]);
    if (!v)
        throw new UndeclaredVariableException(this->m_args[0]);

    state->push_function_arg(v);
    v->release();
    return false;
}

