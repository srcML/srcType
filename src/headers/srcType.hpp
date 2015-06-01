#include <srcTypeHandler.hpp>
#include <srcSAXController.hpp>
#include <iostream> 

std::unordered_map<std::string, std::function<void()>> srcTypeHandler::process_map;
std::unordered_map<std::string, std::function<void()>> srcTypeHandler::process_map2;
class srcType{
    private:
        TypeDictionary dictionary;
    public:
        srcType(const char*, const char*);
        srcType(std::string, const char*);
        srcType(FILE*, const char*);
        srcType(int, const char*);
};


inline std::string OutputQuotedString(std::string str){
  return "\""+str+"\"";
}
inline std::string ConstructNameProfileCall(int linenumber, short int category, std::string type, std::string name, std::string namespacename, bool classMember, bool alias){
    return "NameProfile("+OutputQuotedString(std::to_string(linenumber))+","+OutputQuotedString(std::to_string(category))+","+OutputQuotedString(type)+","+OutputQuotedString(name)+","+OutputQuotedString(namespacename)+","+OutputQuotedString(std::to_string(classMember))+","+OutputQuotedString(std::to_string(alias))+")";
}
inline std::string ConstructFunctionProfileCall(std::string name, std::string fnNamespace, std::string returnType, std::string returnTypeNamespace){
    return "FunctionProfile("+OutputQuotedString(name)+","+OutputQuotedString(fnNamespace)+","+OutputQuotedString(returnType)+","+OutputQuotedString(returnTypeNamespace)+",";
}
inline std::string SerializeMapToString(VarTypeMap::const_iterator pr){
  return ConstructNameProfileCall(pr->second.linenumber,pr->second.category,pr->second.type,pr->second.name,pr->second.namespacename,pr->second.classMember,pr->second.alias);
}
inline std::string SerializeMapToString(FunctionVarMap::const_iterator pr){
  std::string str = ConstructFunctionProfileCall(pr->second.name,pr->second.fnNamespace,pr->second.returnType,pr->second.returnTypeNamespace);
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