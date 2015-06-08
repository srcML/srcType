#ifndef SRCTYPE_HPP
#define SRCTYPE_HPP

#include <srcTypeHandler.hpp>
#include <srcSAXController.hpp>
#include <iostream> 

namespace srcTypeNS{
    
    inline std::string OutputQuotedString(std::string str){
        return "\""+str+"\"";
    }
    inline std::string ConstructNameProfileCall(int linenumber, short int category, std::string type, std::string name, std::string namespacename, bool classMember, bool alias){
        return "NameProfile("+OutputQuotedString(std::to_string(linenumber))+","+OutputQuotedString(std::to_string(category))+","+OutputQuotedString(type)+","+OutputQuotedString(name)+","+OutputQuotedString(namespacename)+","+OutputQuotedString(std::to_string(classMember))+","+OutputQuotedString(std::to_string(alias))+")";
    }
    inline std::string ConstructScopeProfileCall(std::string name, std::string fnNamespace, std::string returnType, std::string returnTypeNamespace){
        return "ScopeProfile("+OutputQuotedString(name)+","+OutputQuotedString(fnNamespace)+","+OutputQuotedString(returnType)+","+OutputQuotedString(returnTypeNamespace)+",";
    }
    inline std::string SerializeMapToString(VarTypeMap::const_iterator pr){
        return ConstructNameProfileCall(pr->second.linenumber,pr->second.category,pr->second.type,pr->second.name,pr->second.namespacename,pr->second.classMember,pr->second.alias);
    }
    inline std::string SerializeMapToString(FunctionVarMap::const_iterator pr){
        std::string str = ConstructScopeProfileCall(pr->second.name,pr->second.fnNamespace,pr->second.returnType,pr->second.returnTypeNamespace);
        str = str+"{";
        for(VarTypeMap::const_iterator p = pr->second.vtMap.begin(); p!=pr->second.vtMap.end(); ++p){
            str = str+"\n{"+OutputQuotedString(p->first)+","+SerializeMapToString(p)+"},";
        }
        if(str[str.size()-1] != '{'){
            str.erase(str.size()-1);
        }else{
            str=str+"{}";//corner case, fix better later. Basically if the nameprofile is empty I need to nest an init list in an init list to fix. Like {{}}
        }
        str=str+"})";
        return str;
    }
    inline std::string SerializeMapToString(FunctionVarMap mp){
        std::string str;
        str=str+"{";
        for(FunctionVarMap::const_iterator fvmIt = mp.begin(); fvmIt != mp.end(); ++fvmIt){
        str=str+"{"+OutputQuotedString(fvmIt->first)+","+SerializeMapToString(fvmIt)+"},"+"\n";
        }
        if(str[str.size()-2] != '{'){
        str.erase(str.size()-2);
        }
        str=str+"};";
        return str;
    }
    inline void OutputHppFile(std::string hpp, std::string mapdat){
        std::string guard = "\n#ifndef INCLUDED_SRCTYPEPROFILE_HPP\n"
                            "#define INCLUDED_SRCTYPEPROFILE_HPP\n";
        std::string endguard = "\n#endif\n";
        std::cerr<<guard<<hpp<<mapdat<<endguard;
    }
    
    class srcType{
        TypeDictionary dictionary;
        public:
            srcType();
            srcType(const char*, const char*);
            srcType(std::string, const char*);
            srcType(FILE*, const char*);
            srcType(int, const char*);
            void ReadArchiveFile(std::string);
            int size()const {return dictionary.fvMap.size();}
            SScopeProfile GetScopeProfile() const{
                return SScopeProfile(dictionary.currentContext.currentFunc->second);
            }
            bool SetContext(std::string fn, int linenumber){
                FunctionVarMap::iterator it = dictionary.fvMap.find(fn);
                if(it != dictionary.fvMap.end()){
                    dictionary.currentContext.currentFunc = it;
                    dictionary.currentContext.ln = linenumber;
                    dictionary.currentContext.functionName = fn;
                    return true;
                }
                return false;
            }
            bool SetContext(int linenumber){
                if(dictionary.currentContext.currentFunc != dictionary.fvMap.end()){
                    dictionary.currentContext.ln = linenumber;
                    return true;
                }
                return false;
            }
            //Definition of find that assumes the user didn't give a context (They should just give a context, though, tbh).
            std::pair<bool, NameProfile> Find(std::string funcname, std::string varname, int lineNumber)const{
                FunctionVarMap::const_iterator fvmIt = dictionary.fvMap.find(funcname);
                if(fvmIt != dictionary.fvMap.end()){
                    VarTypeMap::const_iterator vtmIt = fvmIt->second.vtMap.find(varname+std::to_string(lineNumber));
                    if(vtmIt != fvmIt->second.vtMap.end()){
                        return std::make_pair(true, vtmIt->second);
                    }
                }
                return std::make_pair(false, NameProfile());
            }
            //Definition of find that uses the context (so it doesn't need to take a function name as context)
            std::pair<bool, NameProfile> Find(std::string varname) const{
                if(!dictionary.currentContext.IsSet()){
                    throw std::runtime_error("Context not set"); //for now, std exception
                }else{
                    VarTypeMap::const_iterator it = dictionary.currentContext.currentFunc->second.vtMap.find(varname+std::to_string(dictionary.currentContext.ln));
                    if(it != dictionary.currentContext.currentFunc->second.vtMap.end()){
                        return std::make_pair(true, it->second);
                    }
                    return std::make_pair(false, NameProfile());
                }
            }
            bool Insert(std::string funcname, const NameProfile& np){
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
            bool Insert(const NameProfile& np){
                if(dictionary.currentContext.ln == -1){
                    throw std::runtime_error("Context not set"); //for now, std exception
                }else{
                    auto it = dictionary.currentContext.currentFunc->second.vtMap.find(np.name+std::to_string(np.linenumber));
                    if(it != dictionary.currentContext.currentFunc->second.vtMap.end()){
                        it->second = np;
                        return true;
                    }else{
                        dictionary.currentContext.currentFunc->second.vtMap.insert(std::make_pair(np.name, np));
                        return true;
                    }
                }
                return false;
            }
    
            //output function takes a user-defined output function. I'll provide several defaults for xml/compiled map/whatever else. For now it's pretty dumb.
            void SerializeMap(std::function<std::string(FunctionVarMap)> func){
                std::string serializedMap = "FunctionVarMap srcTypeMap = "+func(dictionary.fvMap);
                OutputHppFile(hppfile, serializedMap);
            }
    };
}
#endif