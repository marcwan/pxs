#include "vm.h"

#include "constantparser.h"
#include "instruction.h"
#include "varpool.h"

using namespace std;



MathopInstruction::MathopInstruction(InstructionCode ic) : Instruction(), m_code(ic) {
}

MathopInstruction::~MathopInstruction() {
}


void MathopInstruction::execute(IExecutionState *state, ScopeStack *scope_stack) {

    this->parse_and_validate_params(scope_stack);
    Variable *result;

    switch (this->m_code) {
        case kInstADD:
            result = this->m_left->add(this->m_right); break;
        case kInstSUB:
            result = this->m_left->subtract(this->m_right); break;
        case kInstMUL:
            result = this->m_left->multiply(this->m_right); break;
        case kInstDIV:
            result = this->m_left->divide(this->m_right); break;
        case kInstMOD:
            result = this->m_left->modulo(this->m_right); break;
        default:
            throw InternalErrorException("Unknown math operation.");
    }

    cerr << this->m_args[2] << " " << endl;
    Varpool *scope = scope_stack->find_scope_for_name(this->m_args[2]);
    if (!scope)
        throw InternalErrorException("Variable undeclared / has no scope?");

    scope->set_value_for_name(this->m_args[2], result);
    scope->release();
    result->release();
    this->m_right->release();
    this->m_left->release();
    this->m_right = this->m_left = NULL;
}


void MathopInstruction::parse_and_validate_params(ScopeStack *scope_stack) {
    this->m_left = Instruction::get_const_or_var(scope_stack, this->m_args[0]);
    if (!this->m_left)
        throw UndeclaredVariableException(this->m_args[0]);
    this->m_right = Instruction::get_const_or_var(scope_stack, this->m_args[1]);
    if (!this->m_right)
        throw UndeclaredVariableException(this->m_args[1]);
}


