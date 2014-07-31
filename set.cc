
#include "vm.h"

#include "constantparser.h"
#include "instruction.h"
#include "varpool.h"

using namespace std;


SetInstruction::SetInstruction() : Instruction() {
}

SetInstruction::~SetInstruction() {
}



/**
 * basic format:  SET var, var_or_const
 */
bool SetInstruction::execute(IExecutionState *state, ScopeStack *scope_stack) {
    if (this->m_args.size() != 2)
        throw new InternalErrorException("Unexpected # args for SET");

    Variable *v = ConstantParser::parse_value(this->m_args[1]);
    if (!v) {
        // it's not a constant, so let's see if it's a variable
        // we have declared.
        v = scope_stack->find_variable_by_name(this->m_args[1]);
        if (!v)
            throw new UndeclaredVariableException(this->m_args[1]);
    }

    if (!scope_stack->set_variable_in_scope(this->m_args[0], v)) 
        throw new UndeclaredVariableException(this->m_args[0]);

    v->release();
    return false;  // continue to next instruction
}

