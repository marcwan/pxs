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
    ExpressionNode *expr;
    StatementSequenceNode *stmts;
};




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

void *if_statement_node(void *expr, void *stmtseq, void *elses) {
    vector<ExpressionStatement> *e = (vector<ExpressionStatement> *)elses;

    ExpressionNode *en = (ExpressionNode *)expr;
    if (en->get_type() != kNodeExpression) {
        cerr << "if (expr) Not an Expression!!!" << endl;
        exit(-1);
    }
    StatementSequenceNode *ssn = (StatementSequenceNode *)stmtseq;
    if (ssn->get_type() != kNodeStatementSequence) {
        cerr << "if (then) Not an Expression!!!" << endl;
        exit(-1);
    }

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
    if (ssn->get_type() != kNodeStatementSequence) {
        cerr << "Dangling else isn't a statement sequence!";
        exit(-1);
    }
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
    es.expr = (ExpressionNode *)exprs;
    es.stmts = (StatementSequenceNode *)stmtseq;

    if (es.expr->get_type() != kNodeExpression
        || es.stmts->get_type() != kNodeStatementSequence) {
        cerr << "elseif didn't get expression or statements eq" << endl;
        exit(-1);
    }

    exist->push_back(es);
    return exist;
}


void *for_loop_node(void *assign, void *expr, void *iter, void *body) {
    StatementNode *assignobj;
    ExpressionNode *exprobj;
    StatementNode *iterobj;
    StatementSequenceNode *bodyobj;

    assignobj = (StatementNode *)assign;
    exprobj = (ExpressionNode *)expr;
    iterobj = (StatementNode *)iter;
    bodyobj = (StatementSequenceNode *)body;

    return new ForLoopNode(assignobj, exprobj, iterobj, bodyobj);
}






void printnode(void *node) {
    ParseNode *pn = (ParseNode *)node;
    cout << pn->to_string() << endl;
}



