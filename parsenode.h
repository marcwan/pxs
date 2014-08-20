#ifndef __PARSENODE_H_
#define __PARSENODE_H_


class StatementSequenceNode;
class ValueNode;

#define INDENT_INCREMENT     4

enum ParseNodeType {
    kNodeFirst = 0,
    kNodeValue = 0,
    kNodeStatement,
    kNodeDeclaration,
    kNodeExpression,
    kNodeFunctionCall,
    kNodeStatementSequence,
    kNodeIfStatement,
    kNodeForLoop,
    kNodeWhileLoop,
    kNodeFunctionDecl,
    kNodeReturn,
    kNodeAssignment,
    kNodeLast = kNodeAssignment
};


#define IS_EXPRESSION(n)          (((n)->get_type() == kNodeExpression) || ((n)->get_type() == kNodeFunctionCall) || ((n)->get_type() == kNodeValue))
#define IS_STMT_SEQ(n)            ((n)->get_type() == kNodeStatementSequence)
#define IS_VALUE_NODE(n)          ((n)->get_type() == kNodeValue)



struct FunctionInfo {
    std::string first_known_namme;
    std::vector<ValueNode *> *params;
    StatementSequenceNode *body;
};  




class ParseNode {
  public:
    ParseNode(ParseNodeType nt) : m_type(nt) { }
    virtual ~ParseNode();

    inline ParseNodeType get_type() { return this->m_type; }
    virtual std::string to_string();
    virtual std::string to_string(int indent) = 0;
  protected:
    ParseNodeType m_type;
};





class StatementNode : public ParseNode {
  public:
    StatementNode(ParseNodeType pnt) : ParseNode(pnt) { }
    virtual std::string to_string(int indent) = 0;
};



class DeclarationNode : public StatementNode {
  public:
    DeclarationNode(std::string v);
    ~DeclarationNode();

    inline void add_var(std::string v) { m_vars.insert(m_vars.begin(), v); }
    inline std::vector<std::string> get_vars() { return m_vars; }

    virtual std::string to_string(int indent);

  protected:
    std::vector<std::string> m_vars;
};

class ExpressionBaseNode : public StatementNode {
  public:
    ExpressionBaseNode(ParseNodeType pnt) : StatementNode(pnt) { }
    virtual std::string to_string(int indent) = 0;
};


class ValueNode : public ExpressionBaseNode {
  public:
    ValueNode(std::string v) : ExpressionBaseNode(kNodeValue), m_value(v) {}
    ~ValueNode();

    virtual std::string to_string(int indent);

    inline std::string get_value() { return this->m_value; }

  protected:
    std::string m_value;
};



class ExpressionNode : public ExpressionBaseNode {
  public:
    ExpressionNode(std::string op, ParseNode *a, ParseNode *b);
    ~ExpressionNode();

    virtual std::string to_string(int indent);

  protected:
    std::string m_op;
    ParseNode *m_a, *m_b;
};


class FunctionCallNode : public ExpressionBaseNode {
  public:
    FunctionCallNode(std::string name, std::vector<ExpressionBaseNode *> *args);
    ~FunctionCallNode();

    virtual std::string to_string(int indent);

  protected:
    std::string m_name;
    std::vector<ExpressionBaseNode *> *m_arglist;
};


class IfStatementNode : public StatementNode {
  public:
    IfStatementNode(ExpressionBaseNode *, StatementSequenceNode *);
    ~IfStatementNode();

    void add_elseif(ExpressionBaseNode *, StatementSequenceNode *);
    void add_else(StatementSequenceNode *);

    virtual std::string to_string(int indent);

  protected:
    std::vector<ExpressionBaseNode *> m_exprs;
    std::vector<StatementSequenceNode *> m_thens;
    StatementSequenceNode *m_else;
};



class ForLoopNode : public StatementNode {
  public:
    ForLoopNode(StatementNode *,
                ExpressionBaseNode *,
                StatementNode *,
                StatementSequenceNode *);
    ~ForLoopNode();

    virtual std::string to_string(int indent);

  protected:
    StatementNode *m_assign;
    ExpressionBaseNode *m_test;
    StatementNode *m_iterator;
    StatementSequenceNode *m_body;
};


class WhileLoopNode : public StatementNode {
  public: 
    WhileLoopNode(ExpressionBaseNode *, StatementSequenceNode *);
    ~WhileLoopNode();

    virtual std::string to_string(int indent);

  protected:
    ExpressionBaseNode *m_expr;
    StatementSequenceNode *m_body;
};


class FunctionDeclarationNode : public ParseNode {
  public:
    FunctionDeclarationNode(const char *name,
                            std::vector<ValueNode *> *paramlist,
                            StatementSequenceNode *stmtseq);
    ~FunctionDeclarationNode();

    virtual std::string to_string(int indent);

  protected:
    std::string m_name;
    std::string m_fntblname;
    std::vector<ValueNode *> *m_params;
    StatementSequenceNode *m_body;
};



class StatementSequenceNode : public ParseNode {
  public:
    StatementSequenceNode() : ParseNode(kNodeStatementSequence) { }
    virtual ~StatementSequenceNode();
    void add_statement(StatementNode *n);

    inline int stmt_count() { return this->m_sequence.size(); }
    virtual std::string to_string(int indent);

  protected:
    std::vector<StatementNode *> m_sequence;
};

class AssignmentNode : public StatementNode {
  public:
    AssignmentNode(const char *lv, ParseNode *rv);
    ~AssignmentNode();

    virtual std::string to_string(int indent);

  protected:
    std::string m_lval;
    ParseNode *m_rval;
};


class ReturnNode : public StatementNode {
  public:
    ReturnNode(ExpressionBaseNode *);
    ~ReturnNode();

    virtual std::string to_string(int indent);

  protected:
    ExpressionBaseNode *m_expr;
};



#endif // __PARSENODE_H_
