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
    struct NameProfile;
    struct ScopeProfile;
    
    typedef std::unordered_map<std::string, NameProfile> VarTypeMap;
    typedef std::unordered_map<std::string, ScopeProfile> FunctionVarMap;
    
    struct NameProfile{
        int linenumber;
        int category;
        std::string type;
        std::string name;
        std::string namespacename;
        bool isConst;
        bool classMember;
        bool alias;
        bool usesIndex;
        NameProfile(){
            usesIndex = false;
            alias = false;
            classMember = false;
            isConst = false;
        }
        void clear(){
            linenumber = 0;
            category = 0;
            type.clear();
            name.clear();
            namespacename.clear();
            classMember = false;
            alias = false;
            isConst = false;
            usesIndex = false;
        }
    };
    struct ScopeProfile{
        std::string name;
        std::string fnNamespace;
        std::string returnType;
        std::string returnTypeNamespace;
        bool isMethod;
        bool isConst;
        bool hasConstReturn;
        bool alias;
        int category;
        VarTypeMap vtMap;
        ScopeProfile(){isMethod = false; isConst = false; hasConstReturn = false; alias = false;}
        
        ScopeProfile(std::string nm):name(nm){isMethod = false; isConst = false; hasConstReturn = false; alias = false;}
        
        ScopeProfile(std::string nm, std::string fnNpace, std::string retType, std::string returnTNpace, std::string isMethd, int cat, bool cnst, bool hcnstret, bool isAlias, VarTypeMap vtm)
        : name(nm), fnNamespace(fnNpace), returnType(retType), returnTypeNamespace(returnTNpace), isMethod(std::stoi(isMethd)), category(cat), isConst(cnst), alias(isAlias),
        hasConstReturn(hcnstret), vtMap(vtm){}

        ScopeProfile operator+=(const ScopeProfile& fp){
            if(!name.empty()){ //currently a function we've already seen
            for(auto it : fp.vtMap){
                vtMap.insert(it);
            }
            }else{
                name = fp.name;
                fnNamespace = fp.fnNamespace;
                returnType = fp.returnType;
                returnTypeNamespace = fp.returnTypeNamespace;
                category = fp.category;
                isConst = fp.isConst;
                isMethod = fp.isMethod;
                hasConstReturn = fp.hasConstReturn;
                alias = fp.alias;
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
            category = 0;
            isConst = false;
            alias = false;
            hasConstReturn = false;
            vtMap.clear();
        }
    };
    struct SScopeProfile{
        std::string name;
        std::string fnNamespace;
        std::string returnType;
        std::string returnTypeNamespace;
        bool isMethod;
        bool isConst;
        bool hasConstReturn;
        bool alias;
        int category;
        SScopeProfile(){isMethod = false; isConst = false; hasConstReturn = false; alias = false;}
        SScopeProfile(std::string nm, std::string fnNpace, std::string retType, std::string returnTNpace, std::string isMethd, int cat, bool cnst, bool hascnstret, bool isAlias)
        : name(nm), fnNamespace(fnNpace), returnType(retType), returnTypeNamespace(returnTNpace), isMethod(std::stoi(isMethd)), category(cat), isConst(cnst), hasConstReturn(hascnstret), alias(isAlias){}
        SScopeProfile(const ScopeProfile& fp){
            name = fp.name;
            fnNamespace = fp.fnNamespace;
            returnType = fp.returnType;
            returnTypeNamespace = fp.returnTypeNamespace;
            isMethod = fp.isMethod;
            category = fp.category;
            isConst = fp.isConst;
            hasConstReturn = fp.hasConstReturn;
            alias = fp.alias;
        }
    };
    
    struct TypeDictionary{
        //context can be used to keep track of what function you're searching in. Makes searching faster because I assume you're using that function as the context
        struct Context{
                std::string functionName;
                int ln;
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