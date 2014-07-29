#ifndef __SCOPESTACK_H_
#define __SCOPESTACK_H_

#include "refcounted.h"
#include "varpool.h"


#define kCompareResetMask    0x80
#define kCompareInvalid      0x20
#define kCompareAlwaysFalse  0x10
#define kCompareIdentical    0x08
#define kCompareEqual        0x04
#define kCompareLessThan     0x02
#define kCompareGreaterThan  0x01
#define kCompareNotEqual     0x00



class ScopeStack : public Refcounted {
  public:
    
    void push_pool(Varpool *);
    Varpool *pop_pool();

    ScopeStack();
    ScopeStack(const ScopeStack &copyme);
    ~ScopeStack();

    // if it's in the scope stack, set it, else declare and set in top scope.
    virtual Varpool *get_current_scope();
    virtual Variable *find_variable_by_name(std::string var_name, bool go_deep = true);
    virtual Varpool *find_scope_for_name(std::string var_name);
    virtual bool set_variable_in_scope(std::string, Variable *);

    inline byte get_compare_flags() { return this->m_last_compare_flags; }
    inline void set_compare_flags(byte f) { this->m_last_compare_flags = f; }

  protected:
    std::vector<Varpool *> m_scopes;
    byte m_last_compare_flags;
};



#endif //  __SCOPESTACK_H_
