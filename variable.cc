#include "vm.h"
#include "variable.h"


using namespace std;

const char *kTypeNameNumber   = "number";
const char *kTypeNameString   = "string";
const char *kTypeNameBoolean  = "boolean";
const char *kTypeNameArray    = "array";
const char *kTypeNameObject   = "object";
const char *kTypeNameFunction = "function";
const char *kTypeNameUnknown  = "unknown";

struct TypeNameMappings {
    const char *name;
    VariableType type;
} const g_mappings [] = {
    { kTypeNameNumber, kTypeNumber },
    { kTypeNameString, kTypeString },
    { kTypeNameBoolean, kTypeBoolean },
    { kTypeNameArray, kTypeArray },
    { kTypeNameObject, kTypeObject },
    { kTypeNameFunction, kTypeFunction },
    { kTypeNameUnknown, kTypeUnknown }
};


Variable *Variable::create_for_type(VariableType t) {
    switch (t) {
        case kTypeNumber: return new Number();
        case kTypeUnknown: return new Unknown();
        default:
            throw InternalErrorException("NOT IMPLEMENTED YET");
    }
    
}





Variable::Variable(VariableType type) : Refcounted() {
    m_type = type;
}

Variable::~Variable() {
}




VariableType string_to_variable_type(string typestr) {
    for (int i = 0; i < sizeof(g_mappings) / sizeof(struct TypeNameMappings); i++) {
        if (typestr.compare(g_mappings[i].name) == 0)
            return g_mappings[i].type;
    }

    throw InvalidVariableTypeException(typestr);
}

string variable_type_to_string(VariableType type) {
    for (int i = 0; i < sizeof(g_mappings) / sizeof(struct TypeNameMappings); i++) {
        if (g_mappings[i].type == type)
            return g_mappings[i].name;
    }

    throw InternalErrorException("Variable has an unknown type %d", type);
}




OperationForTypeNotSupported::OperationForTypeNotSupported(std::string op,
                                                           std::string mytype,
                                                           VariableType other_type) {
    char buf[256];
    snprintf(buf, sizeof(buf), "Can't perform operator %s for %s and %s",
             op.c_str(), mytype.c_str(),
             variable_type_to_string(other_type).c_str());
    this->message = buf;
}
