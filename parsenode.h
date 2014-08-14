#ifndef __PARSENODE_H_
#define __PARSENODE_H_


#define INDENT_INCREMENT     4

enum ParseNodeType {
    kNodeFirst = 0,
    kNodeValue = 0,
    kNodeStatement,
    kNodeDeclaration,
    kNodeExpression,
    kNodeStatementSequence,
    kNodeIfStatement,
    kNodeAssignment,
    kNodeLast = kNodeAssignment
};


class StatementSequenceNode;



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



class ValueNode : public ParseNode {
  public:
    ValueNode(std::string v) : ParseNode(kNodeValue), m_value(v) {}
    ~ValueNode();

    virtual std::string to_string(int indent);


    inline std::string get_value() { return this->m_value; }

  protected:
    std::string m_value;
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

class ExpressionNode : public StatementNode {
  public:
    ExpressionNode(std::string op, ParseNode *a, ParseNode *b);
    ~ExpressionNode();

    virtual std::string to_string(int indent);


  protected:
    std::string m_op;
    ParseNode *m_a, *m_b;
};

class IfStatementNode : public StatementNode {
  public:
    IfStatementNode(ExpressionNode *, StatementSequenceNode *);
    ~IfStatementNode();

    virtual std::string to_string(int indent);

  protected:
    std::vector<ExpressionNode *> m_exprs;
    std::vector<StatementSequenceNode *> m_thens;
    StatementSequenceNode *m_else;
};


class StatementSequenceNode : public ParseNode {
  public:
    StatementSequenceNode() : ParseNode(kNodeStatementSequence) { }
    virtual ~StatementSequenceNode();
    void add_statement(StatementNode *n);

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


#endif // __PARSENODE_H_
