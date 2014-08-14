#include "vm.h"

#include "instruction.h"
#include "varpool.h"
#include <stdio.h>

using namespace std;

//const char *kNameDebugFlags = ":::FLAGS:::";



DebugPrintInstruction::DebugPrintInstruction() : Instruction() {
}

DebugPrintInstruction::~DebugPrintInstruction() {
}


bool DebugPrintInstruction::execute(IExecutionState *state, ScopeStack *scope_stack) {
    // find out which variable it is
/*    if (this->m_args[0].compare(kNameDebugFlags) == 0) {
        byte b = state->get_compare_flags();
        char buf[200];
        snprintf(buf, sizeof(buf),
                 "DEBUGPRINT COMPARE FLAGS: 0x%x\n", b);
        cout << buf;
    } else {
*/
        Variable *v = scope_stack->find_variable_by_name(this->m_args[0]);

        if (!v) {
            cout << "DEBUGPRINT: NO SUCH VARIABLE: " << this->m_args[0] << endl;
        } else {
            cout << "DEBUGPRINT: " + this->m_args[0] << " = " << v->to_string() << endl;
            v->release();
        }
//    }

    return false;
}

