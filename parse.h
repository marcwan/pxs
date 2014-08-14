
#ifdef __cplusplus
extern "C" {
#endif


void *value_node(const char *);

void *first_decl(const char *var);
void *add_decl(void *node, const char *var);

void *first_statement();
void *add_statement(void *node, void *statement);
void *create_assignment(void *lv, void *rv);
void *expression_node(const char *op, void *a, void *b);

void *if_statement_node(void *expr, void *stmtseq);


    char *get_temp();


void printnode(void *node);

const char *pop_decls();

void parseprint(const char *format, ...);

void *add_node(const char *name, ...);



#ifdef __cplusplus
}
#endif
