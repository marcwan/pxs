#include "vm.h"

#include "constantparser.h"
#include "instruction.h"
#include "utf8.h"

using namespace std;

//#define PARSE_DEBUG


const char * kInstNameADD = "ADD";
const char * kInstNameCALLFUNC = "CALLFUNC";
//const char * kInstNameCOMPARE = "COMPARE";
const char * kInstNameDEBUGPRINT = "DEBUGPRINT";
const char * kInstNameDECLARE = "DECLARE";
const char * kInstNameDECLAREFN = "DECLAREFN";
const char * kInstNameDIV = "DIV";
const char * kInstNameEXIT = "EXIT";
const char * kInstNameEXITFUNC = "EXITFUNC";
const char * kInstNameJUMP = "JUMP";
const char * kInstNameJUMPF = "JUMPF";
const char * kInstNameJUMPT = "JUMPT";
/*
const char * kInstNameJUMPEQ = "JUMPEQ";
const char * kInstNameJUMPIDENT = "JUMPIDENT";
const char * kInstNameJUMPNEQ = "JUMPNEQ";
const char * kInstNameJUMPNIDENT = "JUMPNIDENT";
const char * kInstNameJUMPGT = "JUMPGT";
const char * kInstNameJUMPGTE = "JUMPGTE";
const char * kInstNameJUMPLT = "JUMPLT";
const char * kInstNameJUMPLTE = "JUMPLTE";
*/
const char * kInstNameMUL = "MUL";
const char * kInstNameMOD = "MOD";
const char * kInstNamePOPARG = "POPARG";
const char * kInstNamePUSHARG = "PUSHARG";
const char * kInstNameREMOVEVAR = "REMOVEVAR";
const char * kInstNameSET = "SET";
const char * kInstNameSUB = "SUB";
const char * kInstNameTESTEQ = "TESTEQ";
const char * kInstNameTESTIDENT = "TESTIDENT";
const char * kInstNameTESTNEQ = "TESTNEQ";
const char * kInstNameTESTNIDENT = "TESTNIDENT";
const char * kInstNameTESTGT = "TESTGT";
const char * kInstNameTESTGTE = "TESTGTE";
const char * kInstNameTESTLT = "TESTLT";
const char * kInstNameTESTLTE = "TESTLTE";


struct InstCodeMapping {
    const char *instruction;
    InstructionCode code;
    int cargs;
} const g_mappings [] = {
    { kInstNameADD, kInstADD, 3 },
//    { kInstNameCOMPARE, kInstCOMPARE, 2},
    { kInstNameCALLFUNC, kInstCALLFUNC, 1},
    { kInstNameDEBUGPRINT, kInstDEBUGPRINT, 1},
    { kInstNameDIV, kInstDIV, 3 },
    { kInstNameDECLARE, kInstDECLARE, 1},
    { kInstNameDECLAREFN, kInstDECLAREFN, 2},
    { kInstNameEXIT, kInstEXIT, 1 },
    { kInstNameEXITFUNC, kInstEXITFUNC, 1},
    { kInstNameJUMP, kInstJUMP, 1 },
    { kInstNameJUMPF, kInstJUMPF, 2 },
    { kInstNameJUMPT, kInstJUMPT, 2 },
/*
    { kInstNameJUMPEQ, kInstJUMPEQ, 1 },
    { kInstNameJUMPIDENT, kInstJUMPIDENT, 1 },
    { kInstNameJUMPNEQ, kInstJUMPNEQ, 1 },
    { kInstNameJUMPNIDENT, kInstJUMPNIDENT, 1 },
    { kInstNameJUMPGT, kInstJUMPGT, 1 },
    { kInstNameJUMPGTE, kInstJUMPGTE, 1 },
    { kInstNameJUMPLT, kInstJUMPLT, 1 },
    { kInstNameJUMPLTE, kInstJUMPLTE, 1 },
*/
    { kInstNameMOD, kInstMOD, 3 },
    { kInstNameMUL, kInstMUL, 3 },
    { kInstNamePOPARG, kInstPOPARG, 1 },
    { kInstNamePUSHARG, kInstPUSHARG, 1 },
    { kInstNameREMOVEVAR, kInstREMOVEVAR, 1},
    { kInstNameSET, kInstSET, 2 },
    { kInstNameSUB, kInstSUB, 3 },
    { kInstNameTESTEQ, kInstTESTEQ, 3 },
    { kInstNameTESTIDENT, kInstTESTIDENT, 3 },
    { kInstNameTESTNEQ, kInstTESTNEQ, 3 },
    { kInstNameTESTNIDENT, kInstTESTNIDENT, 3 },
    { kInstNameTESTGT, kInstTESTGT, 3 },
    { kInstNameTESTGTE, kInstTESTGTE, 3 },
    { kInstNameTESTLT, kInstTESTLT, 3 },
    { kInstNameTESTLTE, kInstTESTLTE, 3 }
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
            case kInstADD:
            case kInstSUB:
            case kInstMUL:
            case kInstDIV:
            case kInstMOD:
                i = new MathopInstruction(g_mappings[inst_idx].code); break;
            case kInstCALLFUNC:
                i = new CallFuncInstruction(); break;
//            case kInstCOMPARE:
//                i = new CompareInstruction(); break;
            case kInstDECLARE:
                i = new DeclareInstruction(); break;
            case kInstDECLAREFN:
                i = new DeclareFnInstruction(); break;
            case kInstDEBUGPRINT:
                i = new DebugPrintInstruction(); break;
            case kInstEXIT:
                i = new ExitInstruction(); break;
            case kInstEXITFUNC:
                i = new ExitFuncInstruction(); break;
            case kInstJUMP:
            case kInstJUMPF:
            case kInstJUMPT:
/*
            case kInstJUMPEQ:
            case kInstJUMPIDENT:
            case kInstJUMPNEQ:
            case kInstJUMPNIDENT:
            case kInstJUMPGT:
            case kInstJUMPGTE:
            case kInstJUMPLT:
            case kInstJUMPLTE:
*/
                i = new JumpInstruction(g_mappings[inst_idx].code); break;
            case kInstPOPARG:
                i = new PopArgInstruction(); break;
            case kInstPUSHARG:
                i = new PushArgInstruction(); break;
            case kInstREMOVEVAR:
                i = new RemoveVarInstruction(); break;
            case kInstSET:
                i = new SetInstruction(); break;
            case kInstTESTEQ:
            case kInstTESTIDENT:
            case kInstTESTNEQ:
            case kInstTESTNIDENT:
            case kInstTESTGT:
            case kInstTESTGTE:
            case kInstTESTLT:
            case kInstTESTLTE:
                i = new TestInstruction(g_mappings[inst_idx].code); break;
        }

        i->m_label = label;
        i->m_line = line;
        i->m_inst_idx = inst_idx;
        i->m_args = args;
    } catch (std::exception *e) {
        if (i) i->release();
        throw e;
    }

    return i;
}





Instruction::Instruction() : Refcounted() {
}
    
Instruction::~Instruction() {
}

string Instruction::debug_string() {
    return m_label + ": " + m_line;
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
    throw new UnknownInstructionException(instruction);
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
            utf8::uint32_t c = utf8::next(it, arglist.end());
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
            throw new InstructionParseException(arglist);
        }
    } else {
        if (g_mappings[inst_idx].cargs != 0) {
            throw new InstructionParseException(arglist);
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


Variable *Instruction::get_const_or_var(ScopeStack *ss, string arg) {
    Variable *v = ConstantParser::parse_value(arg);
    if (v)
        return v;
    else
        return ss->find_variable_by_name(arg);
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

    throw new UnknownInstructionException(inst);
}
