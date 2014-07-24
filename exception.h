#ifndef __EXCEPTION_H_
#define __EXCEPTION_H_

#include <stdexcept>


struct InternalErrorException : public std::exception {
    std::string message;
    InternalErrorException(std::string in) : message(in) {}
    InternalErrorException(std::string m, int val);
    ~InternalErrorException() throw () {} // Updated
    const char *what() const throw() { return message.c_str(); }
};



#endif // __EXCEPTION_H_
