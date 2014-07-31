#include "vm.h"

#include "assemblyloader.h"
#include "engine.h"
#include "instruction.h"
#include "instructionrunner.h"
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
    InstructionRunner *ir;
    AssemblyLoader al;

    // this will add any new functions to the global function pool.
    try {
    ir = al.load_assembly(path, m_function_pool);
    } catch (UnknownInstructionException e) {
        cout << "Unknown instruction: " << e.what() << endl;
        return false;
    } catch (InstructionParseException e) {
        cout << "Incorrect # arguments parsing: " << e.what() << endl;
        return false;
    } catch (MalformedAssemblyException e) {
        cout << "Malformed assembly: " << e.what() << endl;
        return false;
    } catch (AssemblyLoadException e) {
        cerr << "Fatal error processing file: " << e.what() << endl;
        return false;
    }

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
