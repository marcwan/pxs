#ifndef __STRINGUTILS_H_
#define __STRINGUTILS_H_

#include <algorithm>

// trim from start
static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}

static inline std::string lowercase(std::string s) {
    std::string snew = s;
    std::transform(snew.begin(), snew.end(), snew.begin(), ::tolower);
    return snew;
}

static inline void lowercaseme(std::string &s) {
}


#endif // __STRINGUTILS_H_
