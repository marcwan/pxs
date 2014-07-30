#include "vm.h"

#include <stdio.h>

#include "engine.h"
#include "instruction.h"
#include "instructionrunner.h"
#include "scopestack.h"


using namespace std;


int main (int argc, char **argv) {

    if (argc != 2) {
        cout << "\nUsage: " << argv[0] << " filename\n";
        return 0;
    }

    Engine engine;
    try {
        engine.init();
    } catch (std::exception e) {
        cout << "Unable to initialise engine: " << e.what()
             << ", terminating." << endl;
        return -1;
    }

    if (!engine.parse_assembly_file(argv[1])) {
        return false;
    }

    bool res = engine.run();
        
/*
    for (int i = 0; i < instructions.size(); i++) {
        string lbl = module_stack->jump_to_label();
        if (lbl) {
            
        cerr << "Executing: " << instruction_code_to_string(instructions[i]->get_instruction()) << endl;
        instructions[i]->execute(module_stack);
    }

    // clean up.
    for (int i = 0; i < instructions.size(); i++) {
        delete instructions[i];
    }

    module_stack->pop_pool();
    module_stack->release();
*/
    return 0;
}

