#include "vm.h"

#include "scopestack.h"

using namespace std;


ScopeStack::ScopeStack() {
}

/**
 * Copy constructor will go ahead and add all the existing varpools, as
 * well as addref them.
 */
ScopeStack::ScopeStack(const ScopeStack &copyme) {
    for (int i = 0; i < copyme.m_scopes.size(); i++) {
        // create a copy of the varpool.
        Varpool *p = new Varpool(*(copyme.m_scopes[i]));
        this->m_scopes.push_back(copyme.m_scopes[i]);
    }
}

ScopeStack::~ScopeStack() {
    for (int i = 0; i < this->m_scopes.size(); i++) {
        Varpool *vp = this->m_scopes.back();
        this->m_scopes.pop_back();
        vp->release();
    }
}


void ScopeStack::push_pool(Varpool *vp) {
    if (!vp) throw new invalid_argument("ScopeStack::push_pool -- vp must not be null");

    vp->addref();
    m_scopes.push_back(vp);
}



Varpool *ScopeStack::pop_pool() {
    if (m_scopes.size() == 0)
        throw new InternalErrorException("Being asked to pop an empty scope stack");

    Varpool *vp = this->m_scopes.back();
    vp->release();
    this->m_scopes.pop_back();
    return vp;
}


Varpool *ScopeStack::get_current_scope() {
    if (m_scopes.size() == 0)
        throw new InternalErrorException("Being asked for scope in an empty scope stack");

    Varpool *vp = m_scopes.back();
    vp->addref();
    return vp;
}


Variable *ScopeStack::find_variable_by_name(string var_name, bool go_deep) {
    int start = this->m_scopes.size() - 1;
    int i = start;
    do {
        Variable *v = m_scopes[i]->find_variable_by_name(var_name);
        if (v) {
            // find_variable_by_name already addref'd it, pass ownership on.
            return v;
        }
        i--;
    } while (i >= 0 && go_deep);

    // couldn't find.
    return NULL;
}


Varpool *ScopeStack::find_scope_for_name(string var_name) {
    if (this->m_scopes.size() == 0) return NULL;

    for (int i = this->m_scopes.size() - 1; i >= 0; i--) {
        Variable *v = m_scopes[i]->find_variable_by_name(var_name);
        if (v) {
            v->release(); // didn't really want it.
            m_scopes[i]->addref();
            return m_scopes[i];
        }
    }

    return NULL;
}


bool ScopeStack::set_variable_in_scope(std::string name, Variable *v) {
    Varpool *vp = this->find_scope_for_name(name);
    if (!vp) {
        return false;
    }

    return vp->set_value_for_var(name, v, false);
}


