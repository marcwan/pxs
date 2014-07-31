#include "vm.h"

#include <stdio.h>

#include "engine.h"
#include "instruction.h"
#include "instructionrunner.h"
#include "scopestack.h"

using namespace std;


int main (int argc, char **argv) {
    if (argc != 2) {
        cout << "\nUsage: " << argv[0] << " filename\n";
        return 0;
    }

    Engine engine;
    try {
        engine.init();
    } catch (std::exception e) {
        cout << "Unable to initialise engine: " << e.what()
             << ", terminating." << endl;
        return -1;
    }

    if (!engine.parse_assembly_file(argv[1])) {
        return -1;
    }

    engine.run();
    return 0;
}

