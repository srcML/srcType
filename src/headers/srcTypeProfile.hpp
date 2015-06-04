#ifndef INCLUDED_SRCTYPEPROFILE_HPP
#define INCLUDED_SRCTYPEPROFILE_HPP

#include <unordered_map>
#include <string>
#include <utility.hpp>
#include <exception>
#include <functional>
#include <iostream>

namespace srcTypeNS{
    class srcTypeHandler;
    struct NameProfile;
    struct FunctionProfile;
    
    typedef std::unordered_map<std::string, NameProfile> VarTypeMap;
    typedef std::unordered_map<std::string, FunctionProfile> FunctionVarMap;
    
    struct NameProfile{
        int linenumber;
        int category;
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
        bool isMethod;
        VarTypeMap vtMap;
        FunctionProfile(){}
        FunctionProfile(std::string nm, std::string fnNpace, std::string retType, std::string returnTNpace, std::string isMethd, VarTypeMap vtm)
        : name(nm), fnNamespace(fnNpace), returnType(retType), returnTypeNamespace(returnTNpace), isMethod(std::stoi(isMethd)), vtMap(vtm){}
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
            vtMap.clear();
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