#ifndef SRCTYPE_HPP
#define SRCTYPE_HPP

#include <srcTypeHandler.hpp>
#include <srcSAXController.hpp>
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
            //Definition of find that uses the context (so it doesn't need to take a function name as context)
            std::vector<DeclTypePolicy::DeclTypeData> FindVariable(std::string varname) const{
                auto it = data.variableMap.find(currentfilename + currentfunctionname + varname);
                if(it != data.variableMap.end()){
                    return it->second;
                }
            }
            
            //Definition of find that assumes the user didn't give a context (They should just give a context, though, tbh).
            // functionsigdata.name + paramhash + std::to_string(functionsigdata.isConst);
            std::unordered_map<std::string, std::vector<FunctionSignaturePolicy::SignatureData>>::iterator FindFunction(std::string funcname, std::string types, bool isConst) {
                auto it = data.functionMap.find(funcname + types + std::to_string(isConst));
                if(it != data.functionMap.end()){
                    return it;
                }
                return data.functionMap.end();
            }
            /*
            //Definition of find that uses the context (so it doesn't need to take a function name as context)
            std::pair<bool, VariableProfile> FindVariable(std::string varname) const{
                if(!currentContext.IsSet()){
                    throw std::runtime_error("Function Context not set"); //for now, std exception
                }else{
                    VarTypeMap::const_iterator it = currentContext.currentFunc->second.vtMap.find(varname+std::to_string(currentContext.ln));
                    if(it != currentContext.currentFunc->second.vtMap.end()){
                        return std::make_pair(true, it->second);
                    }
                    return std::make_pair(false, VariableProfile());
                }
            }
            bool Insert(std::string funcname, const VariableProfile& np){
                FunctionVarMap::iterator fvmIt = dictionary.fvMap.find(funcname);
                if(fvmIt != dictionary.fvMap.end()){
                    VarTypeMap::iterator vtmIt = fvmIt->second.vtMap.find(np.name+std::to_string(np.linenumber));
                    if(vtmIt != fvmIt->second.vtMap.end()){
                        vtmIt->second = np;
                        return true;
                    }else{
                        fvmIt->second.vtMap.insert(std::make_pair(np.name, np));
                        return true;
                    }
                }
                return false;
            }
            bool Insert(const VariableProfile& np){
                if(currentContext.ln == -1){
                    throw std::runtime_error("Context not set"); //for now, std exception
                }else{
                    auto it = currentContext.currentFunc->second.vtMap.find(np.name+std::to_string(np.linenumber));
                    if(it != currentContext.currentFunc->second.vtMap.end()){
                        it->second = np;
                        return true;
                    }else{
                        currentContext.currentFunc->second.vtMap.insert(std::make_pair(np.name, np));
                        return true;
                    }
                }
                return false;
            }
    */
    };
}
#endif