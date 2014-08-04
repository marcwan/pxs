

#ifdef __cplusplus
extern "C" {
#endif

char *get_temp();
const char *pop_decls();
void push_decl(const char *name);
int have_tmps();
const char *remove_tmps();
void parseprint(const char *format, ...);

#ifdef __cplusplus
}
#endif
