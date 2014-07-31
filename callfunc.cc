#include "vm.h"

#include "instruction.h"
#include "varpool.h"

using namespace std;


CallFuncInstruction::CallFuncInstruction() : Instruction() {
}

CallFuncInstruction::~CallFuncInstruction() {
}


bool CallFuncInstruction::execute(IExecutionState *state, ScopeStack *scope_stack) {
    this->parse_and_validate_params();

    Variable *v = scope_stack->find_variable_by_name(this->m_args[0]);
    if (!v)
        throw new NoSuchFunctionException(this->m_args[0]);
    else if (v->get_type() != kTypeFunction) {
        v->release();
        throw new NotAFunctionException(this->m_args[0]);
    }

    state->invoke_function(dynamic_cast<Function *>(v)->get_implementation_name());
    v->release();

    return false; // we didn't modify instruction stream -- continue to next
}



void CallFuncInstruction::parse_and_validate_params() {
    if (this->m_args.size() != 1)
        throw new InternalErrorException("Expecting 1 arg, got: %d", this->m_args.size());
    if (this->m_args[0].length() == 0)
        throw new InternalErrorException("Fn variable name must not be empty.");
}
