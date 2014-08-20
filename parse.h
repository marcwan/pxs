
#ifdef __cplusplus
extern "C" {
#endif


void *value_node(const char *);


void *negative_const_value_node(const char *);
void *negative_identifier_value_node(const char *);

void *first_decl(const char *var);
void *add_decl(void *node, const char *var);

void *first_statement();
void *add_statement(void *node, void *statement);
void *create_assignment(void *lv, void *rv);
void *expression_node(const char *op, void *a, void *b);

void *if_statement_node(void *expr, void *stmtseq, void *elses);
void *danglingelse(void *stmtseq);
void *elseif(void *expr, void *statements, void *existing);

void *for_loop_node(void *assign, void *expr, void *iter, void *body);
void *while_loop_node(void *expr, void *stmts);

void *function_declaration(const char *name, void *param_list, void *stmtseq);
void *first_func_param(void *param);
void *add_func_param(void *list, void *param);


void *function_call_node(const char *name, void *arglist);
void *first_func_arg(void *arg);
void *add_func_arg(void *list, void *arg);


void printnode(void *node);


#ifdef __cplusplus
}
#endif
