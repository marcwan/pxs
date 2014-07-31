#include "vm.h"

#include "instruction.h"
#include "varpool.h"

using namespace std;


ExitInstruction::ExitInstruction() : Instruction() {
}

ExitInstruction::~ExitInstruction() {
}



bool ExitInstruction::execute(IExecutionState *state, ScopeStack *scope_stack) {
    this->m_exitcode = Instruction::get_const_or_var(scope_stack, this->m_args[0]);
    if (!this->m_exitcode)
        throw new UndeclaredVariableException(this->m_args[0]);

    Variable *v = this->m_exitcode->coerce_to_type(kTypeNumber);
    int code = (int)dynamic_cast<Number *>(v)->get_value();
    v->release();
    state->terminate_execution(code);
    return false;
}

