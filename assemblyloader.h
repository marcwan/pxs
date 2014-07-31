#ifndef __ASSEMBLYLOADER_H_
#define __ASSEMBLYLOADER_H_


#include <fstream>

// forward decls
class InstructionRunner;


class AssemblyLoader {
  public:
    // return instruction runner for this module, put new fns in fn_pool
    InstructionRunner *load_assembly(
        const char *path, 
        std::map<std::string, InstructionRunner *> &fn_pool);

  protected:
    std::ifstream m_stream;
    std::string m_path;

    bool look_for(std::string what, std::string &marker,  bool start_only = false);
    InstructionRunner *parse_until(std::string marker);
    std::string extract_fn_name(std::string from);
};



struct MalformedAssemblyException : public std::exception {
    std::string message;
    MalformedAssemblyException(std::string in) : message(in) {}
    ~MalformedAssemblyException() throw () {} // Updated
    const char *what() const throw() { 
        std::string s("Malformed Assembly: ");
        s += message;
        return s.c_str();
    }
};


struct AssemblyLoadException : public std::exception {
    std::string message;
    AssemblyLoadException(std::string in) : message(in) {}
    ~AssemblyLoadException() throw () {} // Updated
    const char *what() const throw() {
        std::string s("Unable to load Assembly file: ");
        s += message;
        return s.c_str();
    }
};


#endif // __ASSEMBLYLOADER_H_
