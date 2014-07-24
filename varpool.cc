#include "vm.h"

#include <algorithm>
#include <map>

#include "utils.h"
#include "varpool.h"

using namespace std;


Varpool::Varpool() {
}

/** 
 * We have to do a bit of work here to copy over the variables and re-addref
 * them in their new home.  no biggie.
 */
Varpool::Varpool(const Varpool &copyme) {
    for (map<string, Variable *>::const_iterator it = copyme.m_vars.begin();
         it != copyme.m_vars.end();
         ++it){

        // addref the variable since it's getting a new owner.
        it->second->addref();
        this->m_vars[it->first] = it->second;
    }
}

Varpool::~Varpool() {
}


/**
 * returns true if it created the variable, false if it already
 * existed.
 */
bool Varpool::declare_var(string name) {
    if (this->m_vars[name] != NULL) return false;

    // our hash table takes ownership of ref.
    Variable *v = Variable::create_for_type(kTypeUnknown);
    this->m_vars[name] = v;
    return true;
}


string Varpool::get_temp() {
    string name = utils_unique_name_for_map(this->m_tmps, "tmp");

    // map takes ownership of ref.
    Variable *v = Variable::create_for_type(kTypeUnknown);
    m_tmps[name] = v;
    return name;
}


void Varpool::replace_value_for_var(string name, Variable *v) {
    if (this->m_vars[n
}
void Varpool::replace_value_for_temp(Variable *v) {
}




bool Varpool::free_tmp(Variable *var) {
    vector<Variable *>::iterator position = find(m_tmps.begin(), m_tmps.end(), var);
    if (position != m_tmps.end())
        m_tmps.erase(position);
    else
        throw NoSuchTempVariableException();

    return true;
}


bool Varpool::free_var(string name, bool throw_on_failure) {
    if (this->m_vars[name] == NULL) {
        if (throw_on_failure)
            throw NoSuchVariableException(name);
        else
            return false;
    }

    Variable *v = this->m_vars[name];
    this->m_vars.erase(name);
    v->release();
    return true;
}


Variable *Varpool::find_variable_by_name(string var_name) {
    if (this->m_vars[var_name] != NULL) {
        Variable *v = this->m_vars[var_name];
        v->addref();
        return v;
    }

    return NULL;
}

