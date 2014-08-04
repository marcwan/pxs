#include "vm.h"

#include "assemblyloader.h"
#include "engine.h"
#include "instruction.h"
#include "instructionrunner.h"
#include "scopestack.h"

using namespace std;

const char *kNameReturnValue = "::__RETURN_VALUE__::";



void Engine::init() {
    this->m_scope_stack = new ScopeStack();
    if (!this->m_scope_stack)
        throw new InternalErrorException("Out of memory allocating scope stack");

    Varpool *vp = new Varpool();
    this->m_scope_stack->push_pool(vp);
    vp->release();  // stack takes ownership
}



bool Engine::parse_assembly_file(const char *path) {
    InstructionRunner *ir = NULL;
    AssemblyLoader al;

    // this will add any new functions to the global function pool.
    try {
        ir = al.load_assembly(path, m_function_pool);
    } catch (std::exception *e) {
        cout << e->what() << endl;
        delete e;
        return false;
    }

    m_execution_stack.push_back(ir);
    return true;
}

bool Engine::parse_assembly_string(const string &contents) {
    InstructionRunner *ir = NULL;
    AssemblyLoader al;

    // this will add any new functions to the global function pool.
    try {
        ir = al.load_assembly(contents, m_function_pool);
    } catch (std::exception *e) {
        cout << e->what() << endl;
        delete e;
        return false;
    }

    m_execution_stack.push_back(ir);
    return true;
}





bool Engine::run() {
    InstructionRunner *runner;

    while (true) {
        if (m_execution_stack.size() == 0) return true;  // exited no probs
        runner = m_execution_stack.back();

        InstructionResult res = runner->execute_next(this, this->m_scope_stack);
        if (res == kInstResultException)
            return false;
        if (res == kInstResultNoMoreInstructions) {
            m_execution_stack.pop_back();
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


void Engine::push_function_arg(Variable *v) {
    v->addref();
    this->m_arg_stack.push_back(v);
}

Variable *Engine::pop_function_arg() {
    Variable *v = this->m_arg_stack.back();
    this->m_arg_stack.pop_back();

    // transfer refcount to caller.
    return v;
}




void Engine::invoke_function(string impl_name) {
    if (m_function_pool[impl_name] == NULL)
        throw new InternalNoSuchFunctionException(impl_name);

    Varpool *vp = new Varpool();
    this->m_scope_stack->push_pool(vp);
    vp->release();  // stack takes ownership

    m_execution_stack.push_back(m_function_pool[impl_name]);
    m_function_pool[impl_name]->reset();
}


void Engine::exit_function(Variable *retval) {
    this->m_execution_stack.pop_back();
    this->m_scope_stack->pop_pool();

    // now set ::__RETVAL__:: in the current scope.
    Varpool *vp = this->m_scope_stack->get_current_scope();
    vp->set_value_for_name(kNameReturnValue, retval, false);
    vp->release();
}


void Engine::jump_to_label(string label) {
    InstructionRunner *runner;
    if (m_execution_stack.size() == 0)
        throw new InternalErrorException("Asked to jump to label, but no instructions!");
    runner = m_execution_stack.back();

    runner->jump_to_label(label);
}
  

void Engine::terminate_execution(int code) {
    cerr << "IMPLEMENT 'EXIT' FOR REAL YA BUM" << endl;
//    exit(code);
}
