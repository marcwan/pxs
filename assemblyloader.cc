#include "vm.h"

#include "assemblyloader.h"
#include "engine.h"
#include <errno.h>
#include "instruction.h"
#include "instructionrunner.h"
#include <stdio.h>
#include <string.h>

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
    string marker;

    m_path = path;
    m_file_stream.open(path);
    if (!m_file_stream.is_open())
        throw new AssemblyLoadException(strerror(errno));

    m_stream = &m_file_stream;
    return this->load_assembly_from_stream(fn_pool);
}

InstructionRunner *AssemblyLoader::load_assembly
(
    const string &contents,
    map<string, InstructionRunner *> &fn_pool
)
{
    m_string_stream.str(contents);
    m_stream = &m_string_stream;
    return this->load_assembly_from_stream(fn_pool);
}




InstructionRunner *AssemblyLoader::load_assembly_from_stream
(
    map<string, InstructionRunner *> &fn_pool
)
{
    string marker;

    if (!this->look_for(kMarkerModuleBody, marker))
        throw new MalformedAssemblyException("No \"MODULE_BODY\" marker");

    InstructionRunner *body;
    body = this->parse_until(kMarkerEndModuleBody);

    // next, load functions.
    m_stream->seekg(0);
    while (this->look_for(kMarkerFunctionStart, marker, true)) {
        string fnname = this->extract_fn_name(marker);
        string endmarker = kMarkerFunctionEnd + fnname;

        InstructionRunner *fnbody = this->parse_until(endmarker);
        fn_pool[fnname] = fnbody;
        // fn_pool will own ref
    }

    // make caller take ownership of refcount
    return body;
}

    

bool AssemblyLoader::look_for(string what, string &marker, bool start_only) {
    string line;
    int len;
    if (start_only) len = what.length();

    while (getline(*m_stream, line)) {
        trim(line);
        if (start_only) {
            if (line.length() >= len
                && line.substr(0, len).compare(what) == 0) {
                marker = line;
                return true;
            }
        } else {
            if (line.compare(what) == 0) {
                marker = line;
                return true;
            }
        }
    }

    return false;
}


InstructionRunner *AssemblyLoader::parse_until(string marker) {
    string line;
    string last_label("");

    vector<Instruction *> instructions;

    // we are positioned right after the staritng line, read until we get
    // to the end marker
    while (getline(*m_stream, line)) {
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

        } catch (std::exception *e) {
            for (int i = 0; i < instructions.size(); i++) {
                instructions[i]->release();
            }
            throw e;
        }
    }

    InstructionRunner *ir = new InstructionRunner(instructions);
    if (!ir)
        throw new AssemblyLoadException("out of memory parsing instructions.");
    return ir;
}



/**
 * This is unicode safe -- I only skip through the guaranteed
 * ascii chars at the beginning using ++
 */
string AssemblyLoader::extract_fn_name(string from) {
    trim(from);
    return from.substr(strlen(kMarkerFunctionStart));
}
