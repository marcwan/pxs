#include "vm.h"


#include <stdio.h>
#include <fstream>

#include "instruction.h"
#include "scopestack.h"


using namespace std;


int main (int argc, char **argv) {

    if (argc != 2) {
        cout << "\nUsage: " << argv[0] << " filename\n";
        return 0;
    }

    const char *byte_code_path = argv[1];
    ifstream bcf(byte_code_path);
    if (!bcf.is_open()) {
        cout << "Could not open " << byte_code_path << endl;
        return 0;
    }

    unsigned line_count = 1;
    string line;
    string last_label("");

    vector<Instruction *> instructions;

    while (getline(bcf, line)) {

        // first see if this is a label. if so, remember for next
        // line
        if (!std::isspace(line[0])
            && (line[line.length() - 1] == ':')) {
            last_label = line;
            continue;
        }

        trim(line);
        if (line.length() == 0) continue;

        try {
            Instruction *i = Instruction::instruction_from_line(line, last_label);
            last_label = "";
            if (!i) {
                cout << "WAT, Didn't get back an instruction object!!!" << endl;
            }
//cout << "Parsed: " << instruction_code_to_string(i->get_instruction()) << endl;

            instructions.push_back(i);
            
        } catch (UnknownInstructionException e) {
            cout << "Unknown instruction in " << byte_code_path << ": " << e.what() << endl;
            break;
        } catch (InstructionParseException e) {
            cout << "Incorrect # arguments parsing: " << e.what() << endl;
            break;
        }
    }

    /**
     * Once we have the file loaded, time to start executing the code in it.
     * Step 1: Set up a scope stack and a "global" varpool.
     */
    ScopeStack * module_stack = new ScopeStack();
    Varpool *vp = new Varpool();
    module_stack->push_pool(vp);
    vp->release();  // stack takes ownership

    for (int i = 0; i < instructions.size(); i++) {
cerr << "Executing: " << instruction_code_to_string(instructions[i]->get_instruction()) << endl;
        instructions[i]->execute(module_stack);
    }


    // clean up.
    for (int i = 0; i < instructions.size(); i++) {
        delete instructions[i];
    }

    module_stack->pop_pool();
    module_stack->release();
    return 0;
}

