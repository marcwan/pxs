#include "vm.h"

#include "instruction.h"
#include "varpool.h"

using namespace std;


DebugPrintInstruction::DebugPrintInstruction() {
}

DebugPrintInstruction::~DebugPrintInstruction() {
}


void DebugPrintInstruction::execute(ScopeStack *scope_stack) {
    // find out which variable it is
    Variable *v = scope_stack->find_variable_by_name(this->m_args[0]);

    if (!v) {
        cout << "DEBUGPRINT: NO SUCH VARIABLE: " << this->m_args[0] << endl;
    } else {
        cout << "DEBUGPRINT: " + this->m_args[0] << " = " << v->to_string() << endl;
        v->release();
    }
}
