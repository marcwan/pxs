#ifndef __EXECUTIONSTATE_H_
#define __EXECUTIONSTATE_H_


// forward decl
class Variable;

extern const char *kNameReturnValue;


#define kCompareResetMask    0x80
#define kCompareInvalid      0x20
#define kCompareAlwaysFalse  0x10
#define kCompareIdentical    0x08
#define kCompareEqual        0x04
#define kCompareLessThan     0x02
#define kCompareGreaterThan  0x01
#define kCompareNotEqual     0x00



class IExecutionState {
  public:
    virtual byte get_compare_flags() = 0;
    virtual void set_compare_flags(byte flags) = 0;

    virtual void push_function_arg(Variable *) = 0;
    virtual Variable *pop_function_arg() = 0;
    virtual void invoke_function(std::string implname) = 0;
    virtual void exit_function(Variable *) = 0;

    virtual void jump_to_label(std::string label) = 0;
    virtual void terminate_execution(int exitcode) = 0;
};



#endif // __EXECUTIONSTATE_H_
