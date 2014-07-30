#include "vm.h"

#include "instruction.h"
#include "instructionrunner.h"

using namespace std;


InstructionRunner::InstructionRunner(vector<Instruction *> insts)
    : m_instructions(insts), m_position(0) {
}

InstructionRunner::~InstructionRunner() {
    for (int i = 0; i < m_instructions.size(); i++) {
        m_instructions[i]->release();
    }
}


InstructionResult InstructionRunner::execute_next(ScopeStack *ss) {
    if (this->m_position >= this->m_instructions.size())
        return kInstResultNoMoreInstructions;

    // otherwise, let's execute that puppy!
    try {
        this->m_instructions[this->m_position]->execute(ss);
        m_position++;
    } catch (std::exception e) {
        cout << "Fatal error: " << e.what() << endl;
        return kInstResultException;
    }

    return kInstResultOk;
}


void InstructionRunner::jump_to_label(string lbl) {
    for (int i = 0; i < m_instructions.size(); i++) {
        if (m_instructions[i]->get_label().compare(lbl) == 0) {
            this->m_position = i;
            return;
        }
    }

    throw InternalErrorException(string("Asked to jump to non-existant label: ")
 + lbl);
}


