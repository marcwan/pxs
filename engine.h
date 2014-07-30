#ifndef __ENGINE_H_
#define __ENGINE_H_

// forward decls
class ScopeStack;
class InstructionRunner;


class Engine {
  public:
    bool parse_assembly_file(const char *path);
    bool run();
    void init();

  protected:
    std::vector<InstructionRunner *> m_module_stack;
    std::string m_jump_to_label;
    byte m_last_compare_flags;

    ScopeStack *m_scope_stack;
};


#endif // __ENGINE_H_
