#include <stdlib.h>
#include <string>
using namespace std;

bool numeric_string(string val) {
    bool seen_period = false, seene = false;
    int i = 0;

    if (val[0] == '-') i++;

    while (i < val.length()) {
        if (val[i] == '.') {
            if (seen_period) return false;
            seen_period = true;
        } else if (tolower(val[i]) == 'e') {
            if (seene) return false;
            seene = true;
            if (i + 1 < val.length()
                && (val[i+1] == '-' || val[i+1] == '+')) {
                // cool.
                i++;
            }
        } else if (!isdigit(val[i]))
            return false;

        i++;
    }
    return true;
}


int main (void) {

    fprintf(stderr, "%s  %d\n", "123", numeric_string("123"));
    fprintf(stderr, "%s  %d\n", "123.456", numeric_string("123.456"));
    fprintf(stderr, "%s  %d\n", "-1123", numeric_string("-1123"));
    fprintf(stderr, "%s  %d\n", "-123.345", numeric_string("-123.456"));
    fprintf(stderr, "%s  %d\n", "-.234", numeric_string("-.234"));
    fprintf(stderr, "%s  %d\n", "6e21", numeric_string("6e21"));
    fprintf(stderr, "%s  %d\n", "6e+21", numeric_string("6e+21"));
    fprintf(stderr, "%s  %d\n", "6e-21", numeric_string("6e-21"));
    fprintf(stderr, "%s  %d\n", "-5e+21", numeric_string("-5e+21"));
    fprintf(stderr, "%s  %d\n", "0.00000023423", numeric_string("0.00000023423"));

    fprintf(stderr, "%s  %d\n", "6e21asdf", numeric_string("6e21asdf"));
    fprintf(stderr, "%s  %d\n", "6e21+1", numeric_string("6e2e1+1"));
    fprintf(stderr, "%s  %d\n", "123.123.123", numeric_string("123.123.123"));

    return 0;
}
