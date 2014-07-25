#ifndef __INSTRUCTION_H_
#define __INSTRUCTION_H_

#include "scopestack.h"

/**
 * Forward decl to let the copmiler know this class exists.
 */
class Varpool;


extern const char * kInstNameGETTEMP;
extern const char * kInstNameFREETEMP;
extern const char * kInstNameADD;
extern const char * kInstNameSUB;
extern const char * kInstNameMUL;
extern const char * kInstNameDIV;
extern const char * kInstNameMOD;
extern const char * kInstNameDEBUGPRINT;
extern const char * kInstNameADDVAR;
extern const char * kInstNameREMOVEVAR;
extern const char * kInstNameSET;
extern const char * kInstNameEXIT;

typedef enum InstructionCode {
    kInstGETTEMP,
    kInstFREETEMP,
    kInstADD,
    kInstSUB,
    kInstMUL,
    kInstDIV,
    kInstMOD,
    kInstDEBUGPRINT,
    kInstADDVAR,
    kInstREMOVEVAR,
    kInstSET,
    kInstEXIT
} InstructionCode;

std::string instruction_code_to_string(InstructionCode);
InstructionCode string_to_instruction_code(std::string);

class Instruction {
  public:
    static Instruction *instruction_from_line(std::string line, std::string label);
    virtual ~Instruction();

    std::string get_label();
    std::string get_line();
    InstructionCode get_instruction();
    std::vector<std::string> get_arguments();

    /**
     * run this puppy. will throw one of many exceptions if things go
     * horribly wrong.
     */
   virtual void execute(ScopeStack *) = 0;

  protected:
    Instruction();

    std::string m_line;
    std::string m_label;

    int m_inst_idx;
    std::vector<std::string> m_args;

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
#include "debugprint.h"
#include "exit.h"
#include "freetemp.h"
#include "gettemp.h"
#include "mathop.h"
#include "removevar.h"
#include "set.h"



#endif // __INSTRUCTION_H_
