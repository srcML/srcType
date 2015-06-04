#ifndef TYPEDICTHPP
#define TYPEDICTHPP
namespace srcTypeNS{
    enum PrimTypes{INT, UINT, SIINT, SHINT, SISINT, USISINT, LINT, SLINT, USILINT, SILINT, LDOUBLE, WCHAR, CHAR, UCHAR, SCHAR, VOID, FLOAT, BOOL, DOUBLE, SIZE_T};
    enum VarCategory {primitive, userdefined};
    static const std::string hppfile =
    "\n#include <unordered_map>"
    "\n#include <string>\n"
    "struct NameProfile;"
    "struct FunctionProfile;"
    "typedef std::unordered_map<std::string, NameProfile> VarTypeMap;"
    "typedef std::unordered_map<std::string, FunctionProfile> FunctionVarMap;"
    "enum {primitive, userdefined};"
    "struct NameProfile{"
    "int linenumber;"
    "short int category;"
    "std::string type;"
    "std::string name;"
    "std::string namespacename;"
    "bool classMember;"
    "bool alias;"
    "NameProfile(){}"
    "NameProfile(std::string lnum, std::string cat, std::string typen, std::string nm, std::string nspacename, std::string classmem, std::string ali){"
        "linenumber = std::stoi(lnum);"
        "category = std::stoi(cat);"
        "type = typen;"
        "name = nm;"
        "namespacename = nspacename;"
        "classMember = std::stoi(classmem);"
        "alias = std::stoi(ali);"
    "}"
    "void clear(){"
        "linenumber = 0;"
        "category = 0;"
        "type.clear();"
        "name.clear();"
        "namespacename.clear();"
        "classMember = false;"
        "alias = false;"
    "}"
    "};"
    "struct FunctionProfile{"
    "std::string name;"
    "std::string fnNamespace;"
    "std::string returnType;"
    "std::string returnTypeNamespace;"
    "VarTypeMap vtMap;"
    "FunctionProfile(){}"
    "FunctionProfile(std::string nm, std::string fnNpace, std::string retType, std::string returnTNpace, VarTypeMap vtm)"
    ": name(nm), fnNamespace(fnNpace), returnType(retType), returnTypeNamespace(returnTNpace), vtMap(vtm){}"
    "FunctionProfile operator+=(const FunctionProfile& fp){"
        "if(!name.empty()){"
        "for(auto it : fp.vtMap){"
            "vtMap.insert(it);"
        "}"
        "}else{"
        "name = fp.name;"
        "fnNamespace = fp.fnNamespace;"
        "returnType = fp.returnType;"
        "returnTypeNamespace = fp.returnTypeNamespace;"
        "vtMap = fp.vtMap;"
        "}"
        "return *this;"
    "}"
    "void clear(){"
        "returnType.clear();"
        "name.clear();"
        "fnNamespace.clear();"
        "returnTypeNamespace.clear();"
        "vtMap.clear();"
    "}"
    "};"
    "struct TypeDictionary{"
    "FunctionVarMap fvMap;"
    "};"
    "\n";
    static const std::unordered_map<std::string, PrimTypes> cppPrimTypes{
    {"int", INT},
    {"unsigned int", UINT},
    {"signed int", SIINT},
    {"short int", SHINT},
    {"signed short int", SISINT},
    {"unsigned short int", USISINT},
    {"long int", LINT},
    {"signed long int", SILINT},
    {"unsigned long int", USILINT},
    {"float", FLOAT},
    {"double", DOUBLE},
    {"long double", LDOUBLE},
    {"wchar_t", WCHAR},
    {"char", CHAR},
    {"unsigned char", UCHAR},
    {"signed char", SCHAR},
    {"void", VOID},
    {"float", FLOAT},
    {"bool", BOOL},
    {"double", DOUBLE},
    {"size_t", SIZE_T}};
}
#endif
/*
void OutputDict(const srcTypeHandler& handler){
  std::string str;
  //auto fileNameIt = handler.sysDict.fileTable.find(ffvmIt->first);
  //if(fileNameIt != handler.sysDict.fileTable.end())
  for(FunctionVarMap::const_iterator fvmIt = handler.tDict.fvMap.begin(); fvmIt != handler.tDict.fvMap.end(); ++fvmIt){      
    //std::cerr<<fvmIt->second.name+","<<std::endl;
    for(VarTypeMap::const_iterator vmIt = fvmIt->second.vtMap.begin(); vmIt != fvmIt->second.vtMap.end(); ++vmIt){
      std::cerr<<vmIt->second.name+","<<std::endl;
    }
    std::cerr<<")";
    std::cerr<<"-------------------------"<<std::endl;
  }
}

*/
