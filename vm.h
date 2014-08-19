#ifndef __VM_H_
#define __VM_H_

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "utf8.h"
#include "stringutils.h"
#include "exception.h"
#include "refcounted.h"


typedef unsigned char byte;


// UNDONE: um ... clarify please
#define PXSA_MAX_LINE_LENGTH       2048



#define ASSERT(x, msg)   if (!x) { cerr << "ASSERTION FAILED: \"" << #x << "\"" << endl; cerr << msg << endl; cerr << "Aborting." << endl; exit(-1); }





#endif // __VM_H_
