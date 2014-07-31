#ifndef __SCOPESTACK_H_
#define __SCOPESTACK_H_

#include "refcounted.h"
#include "varpool.h"

class ScopeStack : public Refcounted {
  public:
    
    void push_pool(Varpool *);
    void pop_pool();

    ScopeStack();
    ScopeStack(const ScopeStack &copyme);
    ~ScopeStack();

    // if it's in the scope stack, set it, else declare and set in top scope.
    virtual Varpool *get_current_scope();
    virtual Variable *find_variable_by_name(std::string var_name, bool go_deep = true);
    virtual Varpool *find_scope_for_name(std::string var_name);
    virtual bool set_variable_in_scope(std::string, Variable *);

  protected:
    std::vector<Varpool *> m_scopes;
};



#endif //  __SCOPESTACK_H_
