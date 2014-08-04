#include "vm.h"

#include "parse.h"

using namespace std;

vector<string> decls;

static int ctr = 0;

char *get_temp() {
    char buf[30];
    snprintf(buf, sizeof(buf), "::__TMP%d__::", ++ctr);
    return strdup(buf);
}



void push_decl(const char *name) {
    decls.push_back(string(name));
}


const char *pop_decls() {
    string declstr;
    char holder[1024];

    for (int i = decls.size() - 1; i >= 0; i--) {
        snprintf(holder, sizeof(holder), "\tDECLARE\t%s\n", decls[i].c_str());
        declstr = declstr + string(holder);
    }

    decls.clear();
    return strdup(declstr.c_str());
}


int have_tmps() {
    return ctr > 0;
}

const char *remove_tmps() {
    string remove;

    for (int i = ctr; i > 0; i--) {
        char buf[50];
        snprintf(buf, sizeof(buf), "\tUNDECLARE\t::__TMP%d__::\n", i);
        remove = remove + string(buf);
    }

    ctr = 0;
    return remove.c_str();
}
