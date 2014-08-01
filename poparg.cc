
#include "vm.h"

#include "constantparser.h"
#include "instruction.h"
#include "varpool.h"

using namespace std;


PopArgInstruction::PopArgInstruction() : Instruction() {
}

PopArgInstruction::~PopArgInstruction() {
}



/**
 * basic format:  POPARG var, var_or_const
 */
bool PopArgInstruction::execute(IExecutionState *state, ScopeStack *scope_stack) {
    Variable *v = scope_stack->find_variable_by_name(this->m_args[0]);
    if (!v)
        throw new NoSuchVariableException(this->m_args[0]);
    v->release();

    v = state->pop_function_arg();
    scope_stack->set_variable_in_scope(this->m_args[0], v);
    v->release();
    return false;
}

