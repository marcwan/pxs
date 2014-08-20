#include "vm.h"

#include "compiler.h"
#include "parsenode.h"


using namespace std;


extern map<string, FunctionInfo> g_module_functions;



/** 
 * From LEX and YACC
 */
extern "C" FILE *yyin, *yyout;
extern "C" int yyparse(void **pout);



string compile(const char *fn) {
    yyin = fopen(fn, "r");
    if (!yyin) {
        cout << "Unable to open file: " << fn
             << " (" << strerror(errno) << ")\n";
        exit(-1);
    }

    StatementSequenceNode *body = NULL;
    yyparse((void **)&body);
    fclose(yyin);

    ASSERT((body != NULL), "How does a compiled file not have a body?");

    ostringstream filestr;
    filestr << endl << "MODULE_BODY:\n" 
            << body->to_string(4)
            << "\nEND_MODULE_BODY\n";


    // now generate the bodies
    std::map<std::string, FunctionInfo>::iterator iter;

    for (iter = g_module_functions.begin(); iter != g_module_functions.end(); ++iter) {

        filestr << "\nFUNCTION " << iter->first << endl;
        filestr << iter->second.body->to_string(4);
        filestr << "END_FUNCTION " << iter->first << endl;
    }

    return filestr.str();
}
