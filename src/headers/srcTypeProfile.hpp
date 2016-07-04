#ifndef INCLUDED_SRCTYPEPROFILE_HPP
#define INCLUDED_SRCTYPEPROFILE_HPP

#include <unordered_map>
#include <string>
#include <srctype_utility.hpp>
#include <exception>
#include <functional>
#include <iostream>
namespace srcTypeNS{
    class srcTypeHandler;
    struct VariableProfile;
    struct FunctionProfile;
    
    typedef std::unordered_map<std::string, VariableProfile> VarTypeMap;
    typedef std::unordered_map<std::string, FunctionProfile> FunctionVarMap;
    
    struct VariableProfile{
        int linenumber;
        int isPrimitive;
        std::string type;
        std::string name;
        std::string varNamespace;
        bool isConst;
        bool classMember;
        bool isAlias;
        bool isArray;
        VariableProfile(){
            isArray = false;
            isAlias = false;
            classMember = false;
            isConst = false;
        }
        void clear(){
            linenumber = 0;
            isPrimitive = false;
            type.clear();
            name.clear();
            varNamespace.clear();
            classMember = false;
            isAlias = false;
            isConst = false;
            isArray = false;
        }
    };
    struct FunctionProfile{
        std::string name;
        std::string fnNamespace;
        std::string returnType;
        std::string returnTypeNamespace;
        std::string filename;
        bool isMethod;
        bool isConst;
        bool hasConstReturn;
        bool isAliasReturn;
        int isPrimitive;
        VarTypeMap vtMap;
        FunctionProfile(){isMethod = false; isConst = false; hasConstReturn = false; isAliasReturn = false;}
        
        FunctionProfile(std::string nm):name(nm){isMethod = false; isConst = false; hasConstReturn = false; isAliasReturn = false;}
        
        FunctionProfile(std::string nm, std::string fnNpace, std::string retType, std::string returnTNpace, std::string isMethd, bool cat, bool cnst, bool hcnstret, bool alias, VarTypeMap vtm)
        : name(nm), fnNamespace(fnNpace), returnType(retType), returnTypeNamespace(returnTNpace), isMethod(std::stoi(isMethd)), isPrimitive(cat), isConst(cnst), isAliasReturn(alias),
        hasConstReturn(hcnstret), vtMap(vtm){}

        FunctionProfile operator+=(const FunctionProfile& fp){
            if(!name.empty()){ //currently a function we've already seen
                for(auto it : fp.vtMap){
                    vtMap.insert(it);
                }
            }else{
                filename = fp.filename;
                name = fp.name;
                fnNamespace = fp.fnNamespace;
                returnType = fp.returnType;
                returnTypeNamespace = fp.returnTypeNamespace;
                isPrimitive = fp.isPrimitive;
                isConst = fp.isConst;
                isMethod = fp.isMethod;
                hasConstReturn = fp.hasConstReturn;
                isAliasReturn = fp.isAliasReturn;
                if(!isMethod){
                    isMethod = fp.isMethod;
                }
                vtMap = fp.vtMap;
            }
            return *this;
        }
        void clear(){
            returnType.clear();
            name.clear();
            fnNamespace.clear();
            returnTypeNamespace.clear();
            isMethod = false;
            isPrimitive = false;
            isConst = false;
            isAliasReturn = false;
            hasConstReturn = false;
            vtMap.clear();
        }
    };
    //This struct is basically for when someone just wants function data without variable data. It clips off the variable dictionary to make things a bit faster.
    struct SFunctionProfile{
        std::string name;
        std::string fnNamespace;
        std::string returnType;
        std::string returnTypeNamespace;
        bool isMethod;
        bool isConst;
        bool hasConstReturn;
        bool isAliasReturn;
        int isPrimitive;
        SFunctionProfile(){isMethod = false; isConst = false; hasConstReturn = false; isAliasReturn = false;}
        SFunctionProfile(std::string nm, std::string fnNpace, std::string retType, std::string returnTNpace, std::string isMethd, int cat, bool cnst, bool hascnstret, bool alias)
        : name(nm), fnNamespace(fnNpace), returnType(retType), returnTypeNamespace(returnTNpace), isMethod(std::stoi(isMethd)), isPrimitive(cat), isConst(cnst), hasConstReturn(hascnstret), isAliasReturn(alias){}
        SFunctionProfile(const FunctionProfile& fp){
            name = fp.name;
            fnNamespace = fp.fnNamespace;
            returnType = fp.returnType;
            returnTypeNamespace = fp.returnTypeNamespace;
            isMethod = fp.isMethod;
            isPrimitive = fp.isPrimitive;
            isConst = fp.isConst;
            hasConstReturn = fp.hasConstReturn;
            isAliasReturn = fp.isAliasReturn;
        }
    };
    
    struct TypeDictionary{
        //context can be used to keep track of what function you're searching in. Makes searching faster because I assume you're using that function as the context
        struct Context{
                int ln;
                std::string fileName;
                std::string functionName;
                FunctionVarMap::iterator currentFunc;
                Context(std::string func, unsigned int line, FunctionVarMap::iterator it): functionName(func), ln(line), currentFunc(it){}
                Context():functionName(""), ln(-1){}
                bool IsSet() const {return (ln == -1 || functionName == "") ? false : true;}
        };
        Context currentContext;
        FunctionVarMap fvMap;
    };
}
#endif