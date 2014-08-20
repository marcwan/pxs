#include "vm.h"

#include "parsenode.h"
#include <sstream>

using namespace std;


void   __add_spaces(ostringstream & str, int indent);
string __get_unique_name(string base);


struct FunctionInfo {
    string first_known_namme;
    vector<ValueNode *> *params;
    StatementSequenceNode *body;
};  

/**
 * we'll store all functions in this node here.
 */
map<string, FunctionInfo> g_module_functions;

/**
 * We'll use this to iterate for unique names 
 */
int g_name_iter = 0;



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
    : ExpressionBaseNode(kNodeExpression) {
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
        str << this->m_sequence[i]->to_string(indent) << endl;
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
    ExpressionBaseNode *expr,
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
    __add_spaces(str, indent);
    str << "(then): " << endl;
    str << this->m_thens[0]->to_string(indent + INDENT_INCREMENT);
    for (int i = 1; i < this->m_exprs.size(); i++) {
        __add_spaces(str, indent);
        str << "ELSEIF" << endl;
        __add_spaces(str, indent);
        str << "(expr): " << endl;
        str << this->m_exprs[i]->to_string(indent + INDENT_INCREMENT);
        __add_spaces(str, indent);
        str << "(then): " << endl;
        str << this->m_thens[i]->to_string(indent + INDENT_INCREMENT);
    }

    if (this->m_else) {
        __add_spaces(str, indent);
        str << "(else): " << endl;
        str << this->m_else->to_string(indent + INDENT_INCREMENT);
    }

    return str.str();
}

void IfStatementNode::add_elseif
(
    ExpressionBaseNode *expr,
    StatementSequenceNode *stmtseq
)
{
    if (expr) {
        this->m_exprs.push_back(expr);
        this->m_thens.push_back(stmtseq);
    } else if (stmtseq) {
        this->m_else = stmtseq;
    } else {
        throw new InternalErrorException("add_elseif params are all null");
    }
}


void IfStatementNode::add_else(StatementSequenceNode *stmtseq) {
    this->add_elseif(NULL, stmtseq);
}


ForLoopNode::ForLoopNode
(
    StatementNode *assign,
    ExpressionBaseNode *test,
    StatementNode *iterate,
    StatementSequenceNode *body
)
: StatementNode(kNodeForLoop)
{
    this->m_assign = assign;
    this->m_test = test;
    this->m_iterator = iterate;
    this->m_body = body;
}

ForLoopNode::~ForLoopNode() {

}


string ForLoopNode::to_string(int indent) {
    ostringstream str;
    __add_spaces(str, indent);

    str << "FOR" << endl;
    __add_spaces(str, indent);
    str << "(assign): " << endl;
    str << this->m_assign->to_string(indent + INDENT_INCREMENT);
    str << "(test): " << endl;
    str << this->m_test->to_string(indent + INDENT_INCREMENT);
    str << "(iterate): " << endl;
    str << this->m_iterator->to_string(indent + INDENT_INCREMENT);
    str << "(body): " << endl;
    str << this->m_body->to_string(indent + INDENT_INCREMENT);
    return str.str();
}


WhileLoopNode::WhileLoopNode
(
    ExpressionBaseNode *expr,
    StatementSequenceNode *stmts
)
: StatementNode(kNodeWhileLoop)
{
    this->m_expr = expr;
    this->m_body = stmts;
}

WhileLoopNode::~WhileLoopNode() {
}


string WhileLoopNode::to_string(int indent) {
    ostringstream str;
    __add_spaces(str, indent);

    str << "WHILE" << endl;
    __add_spaces(str, indent);
    str << "(expr): " << endl;
    str << this->m_expr->to_string(indent + INDENT_INCREMENT);
    str << "(body): " << endl;
    str << this->m_body->to_string(indent + INDENT_INCREMENT);
    return str.str();
}




FunctionCallNode::FunctionCallNode(string name, vector<ExpressionBaseNode *> *args)
    : ExpressionBaseNode(kNodeFunctionCall)
{
    this->m_name = name;
    this->m_arglist = args;
}

FunctionCallNode::~FunctionCallNode() {
}


string FunctionCallNode::to_string(int indent) {
    ostringstream str;
    __add_spaces(str, indent);

    str << "FUNCTION CALL (" << this->m_name << "):" << endl;
    if (this->m_arglist) {
        __add_spaces(str, indent);
        int max = this->m_arglist->size() - 1;
        for (int i = max; i >= 0; i--) {
            string s = this->m_arglist->at(i)->to_string(indent + INDENT_INCREMENT);
            __add_spaces(str, indent + INDENT_INCREMENT);
            str << "[" << (max - i) << "]" << s << endl;
        }
    }

    return str.str();
}




FunctionDeclarationNode::FunctionDeclarationNode
(
    const char *name,
    std::vector<ValueNode *> *paramlist,
    StatementSequenceNode *stmtseq
)
: ParseNode(kNodeFunctionDecl)
{
    this->m_name = string(name ? name : "");
    this->m_params = paramlist;
    this->m_body = stmtseq;

    // add the body to the global function pool.
    this->m_fntblname = __get_unique_name(name ? this->m_name : "ANONYMOUS");
    FunctionInfo fi;
    fi.first_known_namme = this->m_name;
    fi.params = this->m_params;
    fi.body = this->m_body;

    g_module_functions[this->m_fntblname] = fi;
}


FunctionDeclarationNode::~FunctionDeclarationNode() {
}


string FunctionDeclarationNode::to_string(int indent) {
    ostringstream str;
    __add_spaces(str, indent);

    str << "DECLAREFN " << this->m_name << ", " << this->m_fntblname << endl;
    __add_spaces(str, indent + INDENT_INCREMENT);
    int size = this->m_params ? this->m_params->size() : 0;
    str << "(expects " << size << " params)";
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


string __get_unique_name(string base) {
    string u = uppercase(trim(base));

    ostringstream str;
    str << "::__" << u << "__" << g_name_iter++ << "__::";
    return str.str();    
}
