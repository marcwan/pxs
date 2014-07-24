#include "vm.h"
#include <ctype.h>

#include "instruction.h"
#include "varpool.h"

using namespace std;



AddInstruction::AddInstruction() {
}

AddInstruction::~AddInstruction() {
}


void AddInstruction::execute(ScopeStack *scope_stack) {

    this->parse_and_validate_params();

    Variable *result = this->m_left->add(this->m_right);
    Varpool *scope = scope_stack->find_scope_for_variable(this->m_args[2]);
    if (!scope) {
        throw InternalErrorException("Variable undeclared / has no scope?");
    }

//    scope->

    for (int i = 0; i < this->m_args.size(); i++) {
        cout << this->m_args[i] << " ";
    }
    cout << endl;
}

void AddInstruction::parse_and_validate_params(ScopeStack *scope_stack) {
    this->m_left = this->get_variable_for_value(this->m_args[0]);
    this->m_right = this->get_variable_for_value(this->m_args[1]);

    this->m_destination = scope_stack->find_variable_by_name(this->m_args[2]);
    if (!this->m_destination) {
        throw UndeclaredVariableException(this->m_args[2]);
    }
}



void AddInstruction::get_variable_for_value(string arg) {
    if (isdigit(arg[0]))
        return new Number(strtold(arg[0]));
    else
        throw InternalErrorException("OTHER TYPES NOT IMPLEMENTED YET");g
}
