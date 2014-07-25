#include "vm.h"

#include "utf8.h"

#include "instruction.h"

using namespace std;

//#define PARSE_DEBUG


const char * kInstNameGETTEMP = "GETTEMP";
const char * kInstNameFREETEMP = "FREETEMP";
const char * kInstNameADD = "ADD";
const char * kInstNameSUB = "SUB";
const char * kInstNameMUL = "MUL";
const char * kInstNameDIV = "DIV";
const char * kInstNameMOD = "MOD";
const char * kInstNameDEBUGPRINT = "DEBUGPRINT";
const char * kInstNameADDVAR = "ADDVAR";
const char * kInstNameREMOVEVAR = "REMOVEVAR";
const char * kInstNameEXIT = "EXIT";
const char * kInstNameSET = "SET";

struct InstCodeMapping {
    const char *instruction;
    InstructionCode code;
    int cargs;
} const g_mappings [] = {
    { kInstNameGETTEMP, kInstGETTEMP, 2  },
    { kInstNameFREETEMP, kInstFREETEMP, 1 },
    { kInstNameADD, kInstADD, 3 },
    { kInstNameSUB, kInstSUB, 3 },
    { kInstNameMUL, kInstMUL, 3 },
    { kInstNameDIV, kInstDIV, 3 },
    { kInstNameMOD, kInstMOD, 3 },
    { kInstNameDEBUGPRINT, kInstDEBUGPRINT, 1},
    { kInstNameADDVAR, kInstADDVAR, 1},
    { kInstNameREMOVEVAR, kInstREMOVEVAR, 1},
    { kInstNameEXIT, kInstEXIT, 1 },
    { kInstNameSET, kInstSET, 2 }
};




Instruction *Instruction::instruction_from_line(string line, string lbl) {
    Instruction *i = NULL;
    try {
        string label = lbl;
        int lastchar = 0;
        int inst_idx;

        trim(label);
        if (label.length()) label = Instruction::parse_label(label);
        inst_idx = Instruction::parse_instruction(line, lastchar);
        vector<string> args = Instruction::parse_arguments(line,
                                                           inst_idx,
                                                           lastchar);

        // it'll have already thrown if the instruction is invalid
        switch (g_mappings[inst_idx].code) {
            case kInstADDVAR:
                i = new AddVarInstruction(); break;
            case kInstADD:
            case kInstSUB:
            case kInstMUL:
            case kInstDIV:
            case kInstMOD:
                i = new MathopInstruction(g_mappings[inst_idx].code); break;
            case kInstDEBUGPRINT:
                i = new DebugPrintInstruction(); break;
            case kInstEXIT:
                i = new ExitInstruction(); break;
            case kInstGETTEMP:
                i = new GetTempInstruction(); break;
            case kInstFREETEMP:
                i = new FreeTempInstruction(); break;
            case kInstREMOVEVAR:
                i = new RemoveVarInstruction(); break;
            case kInstSET:
                i = new SetInstruction(); break;
        }

        i->m_label = label;
        i->m_line = line;
        i->m_inst_idx = inst_idx;
        i->m_args = args;

    } catch (std::exception e) {
        // clean up and re-throw
        if (i) delete i;
        throw e;
    }

    return i;
}





Instruction::Instruction() {
}
    
Instruction::~Instruction() {
}


string Instruction::get_label() {
    return m_label;
}

string Instruction::get_line() {
    return m_line;
}

InstructionCode Instruction::get_instruction() {
    if (m_inst_idx < 0 || m_inst_idx > (sizeof(g_mappings) / sizeof(InstCodeMapping)))
        throw new InternalErrorException("m_inst_idx (%d) out of whack in Instruction", m_inst_idx);
    return g_mappings[m_inst_idx].code;
}

vector<string> Instruction::get_arguments() {
    return vector<string>(m_args);
}



//
//=---------------------------------------------------------------------------=
//=---------------------------------------------------------------------------=
//                            PRIVATE METHODS
//=---------------------------------------------------------------------------=
//=---------------------------------------------------------------------------=
//
string Instruction::parse_label(string lbl) {
    string label = lbl;
    if (label.length() > 0 && label[label.length() - 1] == ':') {
        label = label.substr(0, label.length() - 1);
    }
    return label;
}

int Instruction::parse_instruction(string line, int &lastchar) {
    int start;

    // skip leading space.
    while (lastchar < line.length() && std::isspace(line[lastchar++]));

    // next group of non-space chars form the instruction.
    // this is an ascii string, so don't care about unicode
    // safety.
    start = lastchar - 1;
    while(lastchar < line.length() && !std::isspace(line[lastchar++]));

    string instruction = line.substr(start, lastchar - start - 1);

    for (int i = 0; i < sizeof(g_mappings) / sizeof(InstCodeMapping); i++) {
        if (instruction.compare(g_mappings[i].instruction) == 0) {
#ifdef PARSE_DEBUG
            cout << "found instruction: " << g_mappings[i].instruction << endl;
#endif // PARSE_DEBUG
            return i;
        }
    }

    // didn't find it, it's not an instruction we don't know about yet.
    throw UnknownInstructionException(instruction);
}


vector<string> Instruction::parse_arguments(string line, int inst_idx, int &lastchar) {
    int start;
    string arg;
    vector<string> args;

    // skip leading space.
    while (lastchar < line.length() && std::isspace(line[lastchar++]));
    start = lastchar - 1;

    string arglist = line.substr(start);
    string::iterator it = arglist.begin();
    int distance = utf8::distance(it, arglist.end());
    if (distance) {
        for (int i = 0; i < distance; i++) {
            uint32_t c = utf8::next(it, arglist.end());
            if (c == 0x2c) { // ASCII ","
                trim(arg);
                args.push_back(arg);
                arg = "";
            } else {
                char u[5] = { 0, 0, 0, 0, 0 };
                utf8::append(c, u);
                arg.append(u);
            }
        }
        trim(arg);
        args.push_back(arg);
        arg = "";
        if (args.size() != g_mappings[inst_idx].cargs) {
            throw InstructionParseException(arglist);
        }
    } else {
        if (g_mappings[inst_idx].cargs != 0) {
            throw InstructionParseException(arglist);
        }
    }
   

#ifdef PARSE_DEBUG
    cout << args.size() << endl;

    for (int i = 0; i < args.size(); i++) {
        cout << "'" << args[i] << "' ";
    }
    cout << endl;
#endif // PARSE_DEBUG
    return args;
}








string instruction_code_to_string(InstructionCode code) {
    for (int i = 0; i < sizeof(g_mappings) / sizeof(InstCodeMapping); i++) {
        if (g_mappings[i].code == code)
            return g_mappings[i].instruction;
    }

    return "";
}



InstructionCode string_to_instruction_code(string inst) {
    for (int i = 0; i < sizeof(g_mappings) / sizeof(InstCodeMapping); i++) {
        if (inst.compare(g_mappings[i].instruction) == 0) {
            return g_mappings[i].code;
        }
    }

    throw UnknownInstructionException(inst);
}
