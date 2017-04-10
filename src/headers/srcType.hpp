#ifndef SRCTYPE_HPP
#define SRCTYPE_HPP

#include <srcTypeHandler.hpp>
#include <srcSAXController.hpp>
#include <iostream> 

namespace srcTypeNS{
    class srcType{
        public:
            srcType();
            srcType(const char*, const char*);
            srcType(std::string, const char*);
            srcType(FILE*, const char*);
            srcType(int, const char*);
            void ReadArchiveFile(std::string);
            int size()const {}
            void GetFunctionProfile() const{

            }
            bool SetFunctionContext(std::string file, int lineNumber){

            }
            //Assumes SetFunctionContext was used first
            bool SetVariableContext(std::string fn, int linenumber){

            }
            bool SetVariableContext(int linenumber){

            }
            //Definition of find that assumes the user didn't give a context (They should just give a context, though, tbh).
            void FindFunction(std::string funcname) const{

            }

            //Definition of find that uses the context (so it doesn't need to take a function name as context)
            void FindVariable(std::string varname) const{

            }
            bool Insert(std::string funcname){

            }
            bool Insert(){

            }
    
    };
}
#endif