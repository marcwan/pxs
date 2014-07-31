#include "vm.h"

#include "assemblyloader.h"
#include "engine.h"
#include <errno.h>
#include "instruction.h"
#include "instructionrunner.h"
#include <stdio.h>

using namespace std;


const char *kMarkerModuleBody = "MODULE_BODY:";
const char *kMarkerEndModuleBody = "END_MODULE_BODY";
const char *kMarkerFunctionStart = "FUNCTION ";
const char *kMarkerFunctionEnd = "END_FUNCTION ";


/**
 * basic structure of an assembly file:
 *
 * MODULE_BODY:
 *  [sequence of instructions]
 * END_MODULE_BODY
 *
 * FUNCTION <<FUNCTION_NAME>>
 *  [sequence of instructions]
 * END_FUNCTION <<FUNCTION_NAME>>
 * [repeat functions]
 */
InstructionRunner *AssemblyLoader::load_assembly
(
    const char *path,
    map<string, InstructionRunner *> &fn_pool
)
{
    m_path = path;
    m_stream.open(path);
    if (!m_stream.is_open()) {
        cout << "Could not open \"" << path << "\": " 
             << strerror(errno) << endl;
        return 0;
    }

    if (!this->look_for(kMarkerModuleBody))
        throw MalformedAssemblyException("No \"MODULE_BODY\" marker");

    InstructionRunner *body;
    body = this->read_until(kMarkerEndModuleBody);

    // make caller take ownership of refcount
    return body;
}
    

bool AssemblyLoader::look_for(string what, bool start_only) {
    string line;
    int len;
    if (start_only) len = what.length();

    while (getline(m_stream, line)) {
        trim(line);
        if (start_only) {
            if (line.length() >= len
                && line.substr(0, len).compare(what) == 0) {
                return true;
            }
        } else {
            if (line.compare(what) == 0) return true;
        }
    }

    return false;
}


InstructionRunner *AssemblyLoader::read_until(string marker) {
    string line;
    string last_label("");
    bool errored = false;

    vector<Instruction *> instructions;

    // we are positioned right after the staritng line, read until we get
    // to the end marker
    while (getline(m_stream, line)) {
        // first see if this is a label. if so, remember for next
        // line
        if (!std::isspace(line[0]) && (line[line.length() - 1] == ':')) {
            last_label = line;
            continue;
        }

        // skip empty lines and comment lines
        trim(line);
        if (line.length() == 0) continue;
        if (line[0] == ';') continue;

        // is it our end marker line?
        if (line.compare(marker) == 0) break;

        try {
            Instruction *i = Instruction::instruction_from_line(line, last_label);
            last_label = "";
            if (!i) {
                cerr << "FATALITY: Didn't get an instruction for line:\n" << line << endl;
                return NULL;
            }

            instructions.push_back(i);

        } catch (UnknownInstructionException e) {
            cout << "Unknown instruction: " << e.what() << endl;
            errored = true; break;
        } catch (InstructionParseException e) {
            cout << "Incorrect # arguments parsing: " << e.what() << endl;
            errored = true; break;
        } catch (MalformedAssemblyException e) {
            cout << "Malformed assembly: " << e.what() << endl;
            errored = true; break;
        } catch (AssemblyLoadException e) {
            cerr << "Fatal error processing file: " << e.what() << endl;
            errored = true; break;
        }
    }

    if (errored) {
        for (int i = 0; i < instructions.size(); i++) {
            instructions[i]->release();
        }
        return NULL;
    } else {
        InstructionRunner *ir = new InstructionRunner(instructions);
        if (!ir)
            throw AssemblyLoadException("out of memory parsing instructions.");
        // they take ownership
        return ir;
    }
}
