#ifndef INCLUDED_SRCTYPEPROFILE_HPP
#define INCLUDED_SRCTYPEPROFILE_HPP

#include <unordered_map>
#include <string>
#include <utility.hpp>
#include <exception>
#include <functional>
#include <iostream>

class srcTypeHandler;
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


class TypeDictionary{
    //context can be used to keep track of what function you're searching in. Makes searching faster because I assume you're using that function as the context
    class Context{
        public:
            std::string functionName;
            int ln;
            FunctionVarMap::iterator currentFunc;
            Context(std::string func, unsigned int line, FunctionVarMap::iterator it): functionName(func), ln(line), currentFunc(it){}
            Context():functionName(""), ln(-1){}
            bool IsSet() const {return (ln == -1 || functionName == "") ? false : true;}
    };
    Context currentContext;
    FunctionVarMap fvMap;
    public:
        bool SetContext(std::string fn, int linenumber){
            FunctionVarMap::iterator it = fvMap.find(fn);
            if(it != fvMap.end()){
                currentContext.currentFunc = it;
                currentContext.ln = linenumber;
                currentContext.functionName = fn;
                return true;
            }
            return false;
        }
        bool SetContext(int linenumber){
            if(currentContext.currentFunc != fvMap.end()){
                currentContext.ln = linenumber;
                return true;
            }
            return false;
        }
        //Definition of find that assumes the user didn't give a context (They should just give a context, though, tbh).
        std::pair<bool, NameProfile> Find(std::string funcname, std::string varname, int lineNumber)const{
            FunctionVarMap::const_iterator fvmIt = fvMap.find(funcname);
            if(fvmIt != fvMap.end()){
                VarTypeMap::const_iterator vtmIt = fvmIt->second.vtMap.find(varname+std::to_string(lineNumber));
                if(vtmIt != fvmIt->second.vtMap.end()){
                    return std::make_pair(true, vtmIt->second);
                }
            }
            return std::make_pair(false, NameProfile());
        }
        //Definition of find that uses the context (so it doesn't need to take a function name as context)
        std::pair<bool, NameProfile> Find(std::string varname) const{
            if(currentContext.IsSet()){
                throw std::runtime_error("Context not set"); //for now, std exception
            }else{
                VarTypeMap::const_iterator it = currentContext.currentFunc->second.vtMap.find(varname+std::to_string(currentContext.ln));
                if(it != currentContext.currentFunc->second.vtMap.end()){
                    return std::make_pair(true, it->second);
                }
                return std::make_pair(false, NameProfile());
            }
        }
        bool Insert(std::string funcname, const NameProfile& np){
            FunctionVarMap::iterator fvmIt = fvMap.find(funcname);
            if(fvmIt != fvMap.end()){
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

        //output function takes a user-defined output function. I'll provide several defaults for xml/compiled map/whatever else. For now it's pretty dumb.
        void SerializeMap(std::function<std::string(FunctionVarMap)> func){
            //std::string serializedMap = "FunctionVarMap srcTypeMap = "+func(fvMap);
            //OutputHppFile(hppfile, serializedMap);
        }
        friend srcTypeHandler;
};
#endif