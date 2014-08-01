#include "vm.h"

#include "constantparser.h"
#include <ctype.h>
#include <stdlib.h>
#include "variable.h"

using namespace std;



Variable *ConstantParser::parse_value(string arg) {
    if (isdigit(arg[0]) || (arg[0] == '-' && isdigit(arg[1])))
        return new Number(strtold(arg.c_str(), NULL));
    else if (arg[0] == '"' || arg[0] == '\'')
        return String::from_quoted_literal(arg);
    else if (arg.compare(kValueTrue) == 0)
        return new Boolean(true);
    else if (arg.compare(kValueFalse) == 0)
        return new Boolean(false);
    else if (arg.compare(kValueUndefined) == 0)
        return new Undefined();
    else if (arg.compare(kValueNaN) == 0)
        return new NaN();
    else if (arg.compare(kValueInfinity) == 0)
        return new Infinity(1);
    else if (arg.compare(kValueNegInfinity) == 0)
        return new Infinity(-1);
    return NULL;
}
