#include "vm.h"

#include "instruction.h"
#include "varpool.h"

using namespace std;


ExitInstruction::ExitInstruction() : Instruction() {
}

ExitInstruction::~ExitInstruction() {
}



void ExitInstruction::execute(ScopeStack *scope_stack) {
}

