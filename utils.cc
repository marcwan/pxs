#include <vm.h>

#include <map>
#include <time.h>
#include <stdlib.h>


string utils_unique_name_for_map(const map &m, string base) {
    int len = base.length() + 256;
    char *buf = (char *)malloc(length + 1);
    if (!buf) return "internal_error_out_of_memory";
    do {
        snprintf(buf, length, "%s%d%d", time(NULL), rand());
    } while (map[buf] == NULL);

    return string(buf);
}
