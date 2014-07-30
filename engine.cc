    #include "vm.h"

#include <fstream>
#include "engine.h"
#include "instructionrunner.h"
#include "instruction.h"
#include "scopestack.h"

using namespace std;


void Engine::init() {
    this->m_scope_stack = new ScopeStack();
    if (!this->m_scope_stack)
        throw InternalErrorException("Out of memory allocating scope stack");

    Varpool *vp = new Varpool();
    this->m_scope_stack->push_pool(vp);
    vp->release();  // stack takes ownership
}



bool Engine::parse_assembly_file(const char *path) {
    ifstream bcf(path);
    if (!bcf.is_open()) {
        cout << "Could not open \"" << path << "\"" << endl;
        return 0;
    }

    string line;
    string last_label("");

    vector<Instruction *> instructions;

    while (getline(bcf, line)) {

        // first see if this is a label. if so, remember for next
        // line
        if (!std::isspace(line[0]) && (line[line.length() - 1] == ':')) {
            last_label = line;
            continue;
        }

        // skip empty lines and comment lines
        trim(line);
        if (line.length() == 0) continue;
        if (line[0] == ';') continue;

        try {
            Instruction *i = Instruction::instruction_from_line(line, last_label);
            last_label = "";
            if (!i) {
                cout << "Didn't get an instruction for line:\n" << line << endl;
                return false;
            }

            instructions.push_back(i);
            
        } catch (UnknownInstructionException e) {
            cout << "Unknown instruction in " << path << ": " << e.what() << endl;
            return false;
        } catch (InstructionParseException e) {
            cout << "Incorrect # arguments parsing: " << e.what() << endl;
            return false;
        }
    }

    InstructionRunner *ir = new InstructionRunner(instructions);
    if (!ir) {
        cerr << "Out of memory parsing instructions, terminating." << endl;
        return false;
    }

    // got a stack of instructions. Push that puppy!!
    m_module_stack.push_back(ir);
    // we'll keep the refcount on the ir
    return true;
}



bool Engine::run() {
    InstructionRunner *runner;
    if (m_module_stack.size() == 0) return true;  // exited no probs
    runner = m_module_stack.back();

    while (true) {
        InstructionResult res = runner->execute_next(this, this->m_scope_stack);
        if (res == kInstResultException)
            return false;
        if (res == kInstResultNoMoreInstructions) {
            m_module_stack.pop_back();

            if (m_module_stack.size() == 0)
                return true;
        }
    }

    // deed code
}


/**
 *=---------------------------------------------------------------------------=
 * IExecutionState
 *=---------------------------------------------------------------------------=
 */
byte Engine::get_compare_flags() {
    return this->m_last_compare_flags;
}


void Engine::set_compare_flags(byte flags) {
    this->m_last_compare_flags = flags;
}


void Engine::jump_to_label(string label) {
    InstructionRunner *runner;
    if (m_module_stack.size() == 0)
        throw InternalErrorException("Asked to jump to label, but no instructions!");
    runner = m_module_stack.back();

    runner->jump_to_label(label);
}
  

void Engine::terminate_execution(int code) {
    cerr << "IMPLEMENT 'EXIT' FOR REAL YA BUM" << endl;
//    exit(code);
}
