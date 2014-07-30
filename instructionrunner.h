#ifndef __INSTRUCTION_RUNNER_H_
#define __INSTRUCTION_RUNNER_H_

class Instruction;
class ScopeStack;

enum InstructionResult {
    kInstResultException = -1,
    kInstResultNoMoreInstructions = 0,
    kInstResultOk = 1
};


class InstructionRunner : Refcounted {
  public:
    InstructionRunner(std::vector<Instruction *>);
    ~InstructionRunner();

    InstructionResult execute_next(IExecutionState *, ScopeStack *);
    void jump_to_label(std::string label);

  protected:
    std::vector<Instruction *> m_instructions;
    int m_position;
};



#endif // __INSTRUCTION_RUNNER_H_
