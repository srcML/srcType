#ifndef SRCTYPEDEFS_HPP
#define SRCTYPEDEFS_HPP
struct NameProfile;
struct FunctionProfile;
typedef std::unordered_map<std::string, NameProfile> VarTypeMap;
typedef std::unordered_map<std::string, FunctionProfile> FunctionVarMap;

struct NameProfile{
  int linenumber;
  short int category;
  std::string type;
  std::string name;
  std::string namespacename;
  bool classMember;
  bool alias;
  NameProfile(){}
  NameProfile(std::string lnum, std::string cat, std::string typen, std::string nm, std::string nspacename, std::string classmem, std::string ali){
    linenumber = std::stoi(lnum);
    category = std::stoi(cat);
    type = typen;
    name = nm;
    namespacename = nspacename;
    classMember = std::stoi(classmem);
    alias = std::stoi(ali);
  }
  void clear(){
    linenumber = 0;
    category = 0;
    type.clear();
    name.clear();
    namespacename.clear();
    classMember = false;
    alias = false;
  }
};
struct FunctionProfile{
  std::string name;
  std::string fnNamespace;
  std::string returnType;
  std::string returnTypeNamespace;
  VarTypeMap vtMap;
  FunctionProfile(){}
  FunctionProfile(std::string nm, std::string fnNpace, std::string retType, std::string returnTNpace, VarTypeMap vtm)
  : name(nm), fnNamespace(fnNpace), returnType(retType), returnTypeNamespace(returnTNpace), vtMap(vtm){}
  FunctionProfile operator+=(const FunctionProfile& fp){
    if(!name.empty()){ //currently a function we've already seen
      for(auto it : fp.vtMap){
        vtMap.insert(it);
      }
    }else{
      name = fp.name;
      fnNamespace = fp.fnNamespace;
      returnType = fp.returnType;
      returnTypeNamespace = fp.returnTypeNamespace;
      vtMap = fp.vtMap;
    }
    return *this;
  }
  void clear(){
    returnType.clear();
    name.clear();
    fnNamespace.clear();
    returnTypeNamespace.clear();
    vtMap.clear();
  }
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
#endif