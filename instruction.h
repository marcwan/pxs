#ifndef __INSTRUCTION_H_
#define __INSTRUCTION_H_

#include "executionstate.h"
#include "scopestack.h"

/**
 * Forward decl to let the copmiler know this class exists.
 */
class Varpool;


extern const char * kInstNameADD;
extern const char * kInstNameCALLFUNC;
//extern const char * kInstNameCOMPARE;
extern const char * kInstNameDEBUGPRINT;
extern const char * kInstNameDECLARE;
extern const char * kInstNameDECLAREFN;
extern const char * kInstNameDIV;
extern const char * kInstNameEXIT;
extern const char * kInstNameEXITFUNC;
extern const char * kInstNameJUMP;
extern const char * kInstNameJUMPF;
extern const char * kInstNameJUMPT;
/*
extern const char * kInstNameJUMPEQ;
extern const char * kInstNameJUMPIDENT;
extern const char * kInstNameJUMPNEQ;
extern const char * kInstNameJUMPNIDENT;
extern const char * kInstNameJUMPGT;
extern const char * kInstNameJUMPGTE;
extern const char * kInstNameJUMPLT;
extern const char * kInstNameJUMPLTE;
*/
extern const char * kInstNameMOD;
extern const char * kInstNameMUL;
extern const char * kInstNamePOPARG;
extern const char * kInstNamePUSHARG;
extern const char * kInstNameREMOVEVAR;
extern const char * kInstNameSET;
extern const char * kInstNameSUB;

extern const char * kInstNameTESTEQ;
extern const char * kInstNameTESTIDENT;
extern const char * kInstNameTESTNEQ;
extern const char * kInstNameTESTNIDENT;
extern const char * kInstNameTESTGT;
extern const char * kInstNameTESTGTE;
extern const char * kInstNameTESTLT;
extern const char * kInstNameTESTLTE;

typedef enum InstructionCode {
    kInstADD,
//    kInstCOMPARE,
    kInstCALLFUNC,
    kInstDEBUGPRINT,
    kInstDECLARE,
    kInstDECLAREFN,
    kInstDIV,
    kInstEXIT,
    kInstEXITFUNC,
    kInstJUMP,
    kInstJUMPF,
    kInstJUMPT,
/*
    kInstJUMPEQ,
    kInstJUMPIDENT,
    kInstJUMPNEQ,
    kInstJUMPNIDENT,
    kInstJUMPGT,
    kInstJUMPGTE,
    kInstJUMPLT,
    kInstJUMPLTE,
*/
    kInstMOD,
    kInstMUL,
    kInstPOPARG,
    kInstPUSHARG,
    kInstREMOVEVAR,
    kInstSET,
    kInstSUB,
    kInstTESTEQ,
    kInstTESTIDENT,
    kInstTESTNEQ,
    kInstTESTNIDENT,
    kInstTESTGT,
    kInstTESTGTE,
    kInstTESTLT,
    kInstTESTLTE

} InstructionCode;

std::string instruction_code_to_string(InstructionCode);
InstructionCode string_to_instruction_code(std::string);

class Instruction : public Refcounted {
  public:
    static Instruction *instruction_from_line(std::string line, std::string label);
    virtual ~Instruction();

    std::string debug_string();

    inline std::string get_label() { return this->m_label; }
    inline std::string get_line() { return this->m_line; }
    InstructionCode get_instruction();
    std::vector<std::string> get_arguments();

    /**
     * run this puppy. will throw one of many exceptions if things go
     * horribly wrong. Return true if you changed instruction pointer,
     * else false means we'll just incr to next instruction.
     */
    virtual bool execute(IExecutionState *, ScopeStack *) = 0;

  protected:
    Instruction();

    std::string m_line;
    std::string m_label;

    int m_inst_idx;
    std::vector<std::string> m_args;

    static Variable *get_const_or_var(ScopeStack *, std::string);    

    static std::string parse_label(std::string lbl);
    static int parse_instruction(std::string line, int &lastchar);
    static std::vector<std::string> parse_arguments(std::string line,
                                                    int inst_idx,
                                                    int &lastchar);
};


struct UnknownInstructionException : public std::exception {
    std::string message;
    UnknownInstructionException(std::string in) : message(in) {}
    ~UnknownInstructionException() throw () {} // Updated
    const char *what() const throw() {
        std::string s("Unknown instruction found: ");
        s += message;
        return s.c_str();
    }
};


struct InstructionParseException : public std::exception {
    std::string arguments;
    InstructionParseException(std::string args) : arguments(args)  { }
    ~InstructionParseException() throw () {} // Updated
    const char * what() const throw() {
        std::string s("Incorrect # arguments parsing: ");
        s += arguments;
        return s.c_str();
    }
};


/**
 * We'll include all the instructions here to save people some include hell.
 */
#include "callfunc.h"
//#include "compare.h"
#include "declare.h"
#include "declarefn.h"
#include "debugprint.h"
#include "exit.h"
#include "exitfunc.h"
#include "jump.h"
#include "mathop.h"
#include "poparg.h"
#include "pusharg.h"
#include "removevar.h"
#include "set.h"
#include "test.h"


#endif // __INSTRUCTION_H_
