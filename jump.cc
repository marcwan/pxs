#include "vm.h"

#include "constantparser.h"
#include "instruction.h"
#include "varpool.h"

using namespace std;


JumpInstruction::JumpInstruction(InstructionCode type) : m_op(type) {
}

JumpInstruction::~JumpInstruction() {
}



/**
 * basic format:  JUMP var, var_or_const
 */
void JumpInstruction::execute(ScopeStack *scope_stack) {

}

