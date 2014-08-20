#include "vm.h"

#include "parse.h"
#include "parsenode.h"
#include <stdarg.h>
#include <stdio.h>

using namespace std;

string current_buffer;

vector<string> decls;
static int ctr = 0;





struct ExpressionStatement {
    ExpressionBaseNode *expr;
    StatementSequenceNode *stmts;
};




void *value_node(const char *val) {
    ValueNode *v = new ValueNode(val);
    return (void *)v;
}


void *negative_const_value_node(const char *val) {
    ASSERT(val, "Can't negate NULL, man");
    int len = strlen(val) + 2;
    char *buf = (char *)malloc(len);
    strncpy(buf + 1, val, len - 1);
    free((void *)val);
    buf[0] = '-';
    return value_node(buf);
}

void *negative_identifier_value_node(const char *ident) {
    char *m1 = (char *)malloc(3);
    strncpy(m1, "-1", 3);
    return expression_node("MUL", value_node(ident), value_node(m1));
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



void *if_statement_node(void *expr, void *stmtseq, void *elses) {
    vector<ExpressionStatement> *e = (vector<ExpressionStatement> *)elses;

    ExpressionBaseNode *en = (ExpressionBaseNode *)expr;
    ASSERT(IS_EXPRESSION(en), "IF (expr) wrong type");
    StatementSequenceNode *ssn = (StatementSequenceNode *)stmtseq;
    ASSERT(IS_STMT_SEQ(ssn), "IF (then) not a stmt sequence");

    IfStatementNode *isn = new IfStatementNode(en, ssn);

    // now copy over the elseifs and elses 
    if (e) {
        int max = e->size() - 1;
        for (int i = max; i >= 0; i--) {
            ExpressionStatement es = e->at(i);
            if (es.expr == NULL && i != 0) {
                cerr << "we should only have one dangling else!!" << endl;
                exit(-1);
            }

            isn->add_elseif(es.expr, es.stmts);
        }
    }

    delete e;
    return (void *)isn;
}

void *danglingelse(void *stmtseq) {
    StatementSequenceNode *ssn = (StatementSequenceNode *)stmtseq;
    ASSERT(IS_STMT_SEQ(ssn), "Dangling (else) not a stmt seq");

    vector<ExpressionStatement> *pairs = new vector<ExpressionStatement>();
    ExpressionStatement p;
    p.expr = NULL;
    p.stmts = ssn;

    pairs->push_back(p);
    return pairs;
}

void *elseif(void *exprs, void *stmtseq, void *existing) {
    vector<ExpressionStatement> *exist;
    exist = (vector<ExpressionStatement> *)existing;
    if (!exist) {
        exist = new vector<ExpressionStatement>();
    }

    ExpressionStatement es;
    es.expr = (ExpressionBaseNode *)exprs;
    es.stmts = (StatementSequenceNode *)stmtseq;

    ASSERT(IS_EXPRESSION(es.expr), "ELSEIF (expr) not right");
    ASSERT(IS_STMT_SEQ(es.stmts), "ELSEIF (body) not stmt seq");

    exist->push_back(es);
    return exist;
}



void *for_loop_node(void *assign, void *expr, void *iter, void *body) {
    StatementNode *assignobj;
    ExpressionBaseNode *exprobj;
    StatementNode *iterobj;
    StatementSequenceNode *bodyobj;

    assignobj = (StatementNode *)assign;
    exprobj = (ExpressionBaseNode *)expr;
    iterobj = (StatementNode *)iter;
    bodyobj = (StatementSequenceNode *)body;

    ASSERT(IS_EXPRESSION(exprobj), "FOR (expr) is not an expression");
    ASSERT(IS_STMT_SEQ(bodyobj), "FOR body not a stmt seq");

    return new ForLoopNode(assignobj, exprobj, iterobj, bodyobj);
}

void *while_loop_node(void *expr, void *stmts) {
    ExpressionBaseNode *exprobj = (ExpressionBaseNode *)expr;
    StatementSequenceNode *stmtseq = (StatementSequenceNode *)stmts;

    ASSERT(IS_EXPRESSION(exprobj), "While (expr) is wrong");
    ASSERT(IS_STMT_SEQ(stmtseq), "While (body) is wrong");

    WhileLoopNode *wln = new WhileLoopNode(exprobj, stmtseq);
    return wln;
}





void *function_call_node(const char *name, void *arglist) {
    string fn = string(name);
    vector<ExpressionBaseNode *> *args;
    args = (vector<ExpressionBaseNode *> *)arglist;

    FunctionCallNode *fcn = new FunctionCallNode(fn, args);
    return fcn;
}


void *first_func_arg(void *arg) {
    vector<ExpressionBaseNode *> *args;
    args = new vector<ExpressionBaseNode *>();

    ExpressionBaseNode *ebn = (ExpressionBaseNode *)arg;
    ASSERT(IS_EXPRESSION(ebn), "Func arg isn't expression");
    args->push_back(ebn);
    return args;
}
void *add_func_arg(void *list, void *arg) {
    vector<ExpressionBaseNode *> *args;
    args = (vector<ExpressionBaseNode *> *)list;
    ASSERT((args->size() > 0), "should already have soem args!");

    ExpressionBaseNode *ebn = (ExpressionBaseNode *)arg;
    ASSERT(IS_EXPRESSION(ebn), "fn arg wrong type");

    args->push_back(ebn);
    return args;
}



void *function_declaration(const char *name, void *param_list, void *stmtseq) {
    vector<ValueNode *> *params = (vector<ValueNode *> *)param_list;
    StatementSequenceNode *body = (StatementSequenceNode *)stmtseq;

    ASSERT(IS_STMT_SEQ(body), "Function decl body is wrong");

    FunctionDeclarationNode *fdn;
    fdn = new FunctionDeclarationNode(name, params, body);
    return fdn;
}

void *first_func_param(void *param) {
    vector<ValueNode *> *params;
    params = new vector<ValueNode *>();

    ValueNode *vn = (ValueNode *)param;
    ASSERT(IS_VALUE_NODE(vn), "Func param isn't expression");
    params->push_back(vn);
    return params;
}
void *add_func_param(void *list, void *param) {
    vector<ValueNode *> *params;
    params = (vector<ValueNode *> *)list;
    ASSERT((params->size() > 0), "should already have some params!");

    ValueNode *vn = (ValueNode *)param;
    ASSERT(IS_VALUE_NODE(vn), "fn param wrong type");

    params->push_back(vn);
    return params;
}




void *return_node(void *expr) {
    ExpressionBaseNode *ebn = (ExpressionBaseNode *)expr;
    ASSERT((!expr || IS_EXPRESSION(ebn)), "Returns need an expression");

    return new ReturnNode(ebn);
}




void printnode(void *node) {
    ParseNode *pn = (ParseNode *)node;
    cout << pn->to_string() << endl;
}



