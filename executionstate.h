#ifndef __EXECUTIONSTATE_H_
#define __EXECUTIONSTATE_H_


class IExecutionState {
  public:
    virtual byte get_compare_flags() = 0;
    virtual void set_compare_flags(byte flags) = 0;
    virtual void jump_to_label(std::string label) = 0;
    virtual void terminate_execution(int exitcode) = 0;
};



#endif // __EXECUTIONSTATE_H_
