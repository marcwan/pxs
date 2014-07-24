#include "vm.h"

#include <algorithm>
#include <map>

#include "utils.h"
#include "varpool.h"

using namespace std;

string _unique_name_for_map(const map<string, Variable *> &m, string base);

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
bool Varpool::declare_var(string name, bool throw_if_exists) {
    if (this->m_vars[name] != NULL) {
        if (throw_if_exists)
            throw VariableAlreadyExistsException(name);
        else
            return false;
    }

    // our hash table takes ownership of ref.
    Variable *v = Variable::create_for_type(kTypeUnknown);
    this->m_vars[name] = v;
    return true;
}

/**
 * returns false if the variable did not exist already.
 */
bool Varpool::set_value_for_var(string name, Variable *v, bool throw_if_not_exists) {
    bool existed = (this->m_vars[name] != NULL);
    if (existed)
        this->m_vars[name]->release();  // dead pointer can replace.
    else if (throw_if_not_exists)
        throw NoSuchVariableException(name);

    this->m_vars[name] = v;
    v->addref();
    return existed;
}


bool Varpool::set_value_for_name(string name,
                                 Variable *v,
                                 bool throw_if_not_exists) {
    return this->set_value_for_var(name, v, throw_if_not_exists);
}


bool Varpool::undeclare_var(string name, bool throw_if_not_exists) {
    if (this->m_vars[name] == NULL) {
        if (throw_if_not_exists)
            throw NoSuchVariableException(name);
        else
            return false;
    }

    Variable *v = this->m_vars[name];
    v->release();
    this->m_vars.erase(name);
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



string _unique_name_for_map(const map<string, Variable *> &m, string base) {
    int len = base.length() + 256;
    char *buf = (char *)malloc(len + 1);
    if (!buf) return "internal_error_out_of_memory";
    bool found = true;
    do {
        snprintf(buf, len, "::%s%ld%d", base.c_str(), time(NULL), rand());

        try {
            m.at(buf);
            found = true;
        } catch (out_of_range e) {
            found = false;
        }

    } while (found);

    return string(buf);
}

