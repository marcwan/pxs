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
void SetInstruction::execute(ScopeStack *scope_stack) {
    if (this->m_args.size() != 2)
        throw InternalErrorException("Unexpected # args for SET");

    Variable *v = ConstantParser::parse_value(this->m_args[1]);
    if (!v) {
        // it's not a constant, so let's see if it's a variable
        // we have declared.
        v = scope_stack->find_variable_by_name(this->m_args[0]);
        if (!v)
            throw UndeclaredVariableException(this->m_args[1]);
    }

    if (!scope_stack->set_variable_in_scope(this->m_args[0], v)) 
        throw UndeclaredVariableException(this->m_args[0]);
    v->release();
}

