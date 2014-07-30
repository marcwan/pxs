#ifndef __INSTRUCTION_H_
#define __INSTRUCTION_H_

#include "executionstate.h"
#include "scopestack.h"

/**
 * Forward decl to let the copmiler know this class exists.
 */
class Varpool;


extern const char * kInstNameADD;
extern const char * kInstNameADDVAR;
extern const char * kInstNameCOMPARE;
extern const char * kInstNameDEBUGPRINT;
extern const char * kInstNameDIV;
extern const char * kInstNameMOD;
extern const char * kInstNameMUL;
extern const char * kInstNameREMOVEVAR;
extern const char * kInstNameSET;
extern const char * kInstNameSUB;
extern const char * kInstNameEXIT;
extern const char * kInstNameJUMP;
extern const char * kInstNameJUMPEQ;
extern const char * kInstNameJUMPIDENT;
extern const char * kInstNameJUMPNEQ;
extern const char * kInstNameJUMPNIDENT;
extern const char * kInstNameJUMPGT;
extern const char * kInstNameJUMPGTE;
extern const char * kInstNameJUMPLT;
extern const char * kInstNameJUMPLTE;

typedef enum InstructionCode {
    kInstADD,
    kInstADDVAR,
    kInstCOMPARE,
    kInstDEBUGPRINT,
    kInstDIV,
    kInstMOD,
    kInstMUL,
    kInstREMOVEVAR,
    kInstSET,
    kInstSUB,
    kInstJUMP,
    kInstJUMPEQ,
    kInstJUMPIDENT,
    kInstJUMPNEQ,
    kInstJUMPNIDENT,
    kInstJUMPGT,
    kInstJUMPGTE,
    kInstJUMPLT,
    kInstJUMPLTE,
    kInstEXIT
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
    const char *what() const throw() { return message.c_str(); }
};


struct InstructionParseException : public std::exception {
    std::string arguments;
    InstructionParseException(std::string args) : arguments(args)  { }
    ~InstructionParseException() throw () {} // Updated
    const char * what() const throw() { return arguments.c_str(); }
};


/**
 * We'll include all the instructions here to save people some include hell.
 */
#include "addvar.h"
#include "compare.h"
#include "debugprint.h"
#include "exit.h"
#include "jump.h"
#include "mathop.h"
#include "removevar.h"
#include "set.h"


#endif // __INSTRUCTION_H_
