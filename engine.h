#ifndef __ENGINE_H_
#define __ENGINE_H_

// forward decls
class ScopeStack;
class InstructionRunner;

#include "executionstate.h"


class Engine : public IExecutionState {
  public:
    bool parse_assembly_file(const char *path);
    bool run();
    void init();

    virtual byte get_compare_flags();
    virtual void set_compare_flags(byte);
    virtual void invoke_function(std::string implname);
    virtual void exit_function(Variable *);
    virtual void jump_to_label(std::string);
    virtual void terminate_execution(int code);
  
  protected:
    std::vector<InstructionRunner *> m_execution_stack;
    std::string m_jump_to_label;
    byte m_last_compare_flags;

    std::map<std::string, InstructionRunner *> m_function_pool;

    ScopeStack *m_scope_stack;
};


struct InternalNoSuchFunctionException : public std::exception {
    std::string message;
    InternalNoSuchFunctionException(std::string in) : message(in) {}
    ~InternalNoSuchFunctionException() throw () {} // Updated
    const char *what() const throw() {
        std::string s("(Internal Error) No function with impl named: ");
        s += message;
        return s.c_str();
    }
};


#endif // __ENGINE_H_
