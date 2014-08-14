#include "vm.h"

#include "parse.h"
#include "parsenode.h"
#include <stdarg.h>
#include <stdio.h>

using namespace std;

string current_buffer;

vector<string> decls;
static int ctr = 0;



void *value_node(const char *val) {
    ValueNode *v = new ValueNode(val);
    return (void *)v;
}


void *first_decl(const char *var) {
    DeclarationNode *dn = new DeclarationNode(string(var));
    return (void *)dn;
}

void *add_decl(void *node, const char *var) {
    DeclarationNode *dn = (DeclarationNode *)node;
    dn->add_var(var);
    return dn;
}



void *first_statement() {
    return (void *)new StatementSequenceNode();
}

void *add_statement(void *node, void *statement) {
    StatementSequenceNode *ssn = (StatementSequenceNode *)node;
    ssn->add_statement((StatementNode *)statement);
    return node;
}


void *create_assignment(void *lv, void *rv) {
    const char *lvalue = (const char *)lv;
    ParseNode *r = (ParseNode *)rv;

    AssignmentNode *an = new AssignmentNode(lvalue, r);
    return (void *)an;
}

void *expression_node(const char *op, void *a, void *b) {
    ParseNode *pna = (ParseNode *)a;
    ParseNode *pnb = (ParseNode *)b;

    ExpressionNode *en = new ExpressionNode(op, pna, pnb);
    return (void *)en;
}



void printnode(void *node) {
    ParseNode *pn = (ParseNode *)node;
    cout << pn->to_string() << endl;
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

































char *get_temp() {
    char buf[30];
    snprintf(buf, sizeof(buf), "::__TMP%d__::", ++ctr);
    parseprint("\tDECLARE\t%s\n", buf);
    return strdup(buf);
}




int have_tmps() {
    return ctr > 0;
}

const char *remove_tmps() {
    string remove;

    for (int i = ctr; i > 0; i--) {
        char buf[50];
        snprintf(buf, sizeof(buf), "\tREMOVEVAR\t::__TMP%d__::\n", i);
        remove = remove + string(buf);
    }

    ctr = 0;
    return remove.c_str();
}




void parseprint(const char *format, ...) {
    char buf[PXSA_MAX_LINE_LENGTH];
    va_list arglist;
    va_start(arglist, format);
    vsnprintf(buf, sizeof(buf), format, arglist);

    current_buffer += string(buf);
}
