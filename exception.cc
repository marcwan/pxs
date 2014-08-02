
#include "vm.h"

#include <stdio.h>

InternalErrorException::InternalErrorException(std::string m, int val) {
    char msg[128];
    snprintf(msg, sizeof(msg), m.c_str(), val);
    this->message = msg;
}

