#include "vm.h"

#include "constantparser.h"
#include "instruction.h"
#include "varpool.h"

using namespace std;


JumpInstruction::JumpInstruction(InstructionCode type) : Instruction(), m_op(type) {
}

JumpInstruction::~JumpInstruction() {
}



/**
 * basic format:  JUMP var, var_or_const
 */
void JumpInstruction::execute(IExecutionState *state, ScopeStack *scope_stack) {

}

