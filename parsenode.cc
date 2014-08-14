#include "vm.h"

#include "parsenode.h"
#include <sstream>

using namespace std;


void __add_spaces(ostringstream & str, int indent);




ParseNode::~ParseNode() {
}

string ParseNode::to_string() {
    return this->to_string(0);
}








ValueNode::~ValueNode() {
}

string ValueNode::to_string(int indent) {
    ostringstream str;

    __add_spaces(str, indent);

    str << this->m_value << endl;
    return str.str();
}



DeclarationNode::DeclarationNode(string v) : StatementNode(kNodeDeclaration) {
    m_vars.push_back(v);
}



DeclarationNode::~DeclarationNode() {
}

string DeclarationNode::to_string(int indent) {
    ostringstream str;
    __add_spaces(str, indent);

    str << "DECLARE ";
    int max = this->m_vars.size();
    for (int i = 0; i < max; i++) {
        str << this->m_vars[i];
        if (i < max - 1) str << ", ";
    }

    return str.str();
}


ExpressionNode::ExpressionNode(string op, ParseNode *a, ParseNode *b) 
    : StatementNode(kNodeExpression) {
    this->m_op = op;
    this->m_a = a;
    this->m_b = b;
}

ExpressionNode::~ExpressionNode() {
}

string ExpressionNode::to_string(int indent) {
    ostringstream str;
    __add_spaces(str, indent);

    str << m_op << endl;
    str << m_a->to_string(indent + INDENT_INCREMENT);
    str << m_b->to_string(indent + INDENT_INCREMENT);
    return str.str();
}



StatementSequenceNode::~StatementSequenceNode() {
}

void StatementSequenceNode::add_statement(StatementNode *n) {
    this->m_sequence.push_back(n);
}

string StatementSequenceNode::to_string(int indent) {
    ostringstream str;

    for (int i = 0; i < this->m_sequence.size(); i++) {
        str << this->m_sequence[i]->to_string(indent) << ";" << endl;
    }

    return str.str();
}




AssignmentNode::AssignmentNode(const char *lv, ParseNode *rv)
    : StatementNode(kNodeAssignment) {
    this->m_lval = string(lv);
    this->m_rval = rv;
}


AssignmentNode::~AssignmentNode() {
}

string AssignmentNode::to_string(int indent) {
    ostringstream str;
    __add_spaces(str, indent);

    str << "SET " << endl;
    __add_spaces(str, indent + INDENT_INCREMENT);
    str << this->m_lval << endl;
    str << ((ParseNode *)this->m_rval)->to_string(indent + INDENT_INCREMENT);

    return str.str();
}



IfStatementNode::IfStatementNode
(
    ExpressionNode *expr,
    StatementSequenceNode *stmts
)
: StatementNode(kNodeIfStatement)
{
    m_exprs.push_back(expr);
    m_thens.push_back(stmts);
}

IfStatementNode::~IfStatementNode() {
}

string IfStatementNode::to_string(int indent) {
    ostringstream str;
    __add_spaces(str, indent);

    str << "IF" << endl;
    __add_spaces(str, indent);
    str << "(expr): " << endl;
    str << this->m_exprs[0]->to_string(indent + INDENT_INCREMENT);
    str << "(then): " << endl;
    str << this->m_thens[0]->to_string(indent + INDENT_INCREMENT);
    return str.str();
}






void __add_spaces(ostringstream & str, int indent) {
    char spaces[128];

    for (int j = 0; j < indent; j++) {
        spaces[j] = ' ';
    }
    spaces[indent] = '\0';

    str << spaces;
}
