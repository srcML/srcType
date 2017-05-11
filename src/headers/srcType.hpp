#ifndef SRCTYPE_HPP
#define SRCTYPE_HPP

#include <srcTypeHandler.hpp>
#include <srcSAXController.hpp>
#include <stdexcept>
#include <iostream>

namespace srcTypeNS{
    class srcType{
        public:
            unsigned int currentline;
            std::string currentfilename;
            std::string currentfunctionname;
            srcTypePolicy::srcTypeData data;
            srcType();
            srcType(const char*, const char*);
            srcType(std::string, const char*);
            srcType(FILE*, const char*);
            srcType(int, const char*);
            void ReadArchiveFile(std::string);
            bool IsPrimitive(std::string type){
                return cppPrimTypes.find(type) != cppPrimTypes.end();
            }
            bool SetContext(std::string file, std::string function){
                currentfilename = file;
                currentfunctionname = function;
                return true;
            }
            //Assumes context was set
            std::vector<DeclTypePolicy::DeclTypeData> FindVariable(std::string varName) const{
                auto it = data.variableMap.find(currentfilename + currentfunctionname + varName);
                if(it != data.variableMap.end()){
                    return it->second;
                }else{
                    throw std::runtime_error("Coud not find variable with key: " + currentfilename+" "+currentfunctionname+" "+varName);
                }
            }

            //Does not use context
            std::vector<DeclTypePolicy::DeclTypeData> FindVariable(std::string varName, std::string functionName, std::string fileName) const{
                auto it = data.variableMap.find(fileName + functionName + varName);
                if(it != data.variableMap.end()){
                    return it->second;
                }else{
                    throw std::runtime_error("Coud not find variable with key: " + currentfilename+" "+currentfunctionname+" "+varName);
                }
            }

            std::vector<FunctionSignaturePolicy::SignatureData> FindFunction(std::string funcName, std::string types, bool isConst) {
                auto it = data.functionMap.find(funcName + types + std::to_string(isConst));
                if(it != data.functionMap.end()){
                    return it->second;
                }else{
                    throw std::runtime_error("Coud not find function with key: " + funcName+" "+types+" "+std::to_string(isConst));
                }
            }
    };
}
#endif