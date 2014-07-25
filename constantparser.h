#ifndef __CONSTANTPARSER_H_
#define __CONSTANTPARSER_H_

// forward decl
class Variable;


class ConstantParser {
  public:
    static Variable *parse_value(std::string value);
};


#endif //  __CONSTANTPARSER_H_
