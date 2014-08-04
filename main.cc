#include "vm.h"

#include <algorithm>
#include "engine.h"
#include <errno.h>
#include "instruction.h"
#include "instructionrunner.h"
#include "scopestack.h"
#include <stdio.h>
#include <string.h>

using namespace std;

extern "C" FILE *yyin, *yyout;

extern "C" int yyparse();

const char *exename;

void usage();
string get_file_extension(string fn);



int main (int argc, char **argv) {
    exename = argv[0];
    if (argc != 2)
        usage();

    Engine engine;
    try {
        engine.init();
    } catch (std::exception e) {
        cout << "Unable to initialise engine: " << e.what()
             << ", terminating." << endl;
        return -1;
    }

    // figure out what kind of file we gots.
    string ext = get_file_extension(argv[1]);

    // these are trnasformed into lower case for us
    if (ext == "pxsa") {
        if (!engine.parse_assembly_file(argv[1])) {
            return -1;
        }

        engine.run();
    } else if (ext == "pxs") {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            cout << "Unable to open file: " << argv[1]
                 << " (" << strerror(errno) << ")\n";
            return -1;
        }
        yyparse();
        fclose(yyin);
    }
    return 0;
}



void usage() {
    cout << "usage: " << exename << " filename\n";
    cout << " * filename must have extension .pxs or .pxsa\n";
    exit(1);
}


string get_file_extension(string fn) {
    lowercaseme(fn);
    size_t idx = fn.find_last_of('.');
    if (idx == -1) usage();
    return fn.substr(idx + 1);
}
