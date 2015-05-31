#ifndef INCLUDED_SRCTYPEPROFILE_HPP
#define INCLUDED_SRCTYPEPROFILE_HPP

#include <unordered_map>
#include <map>
#include <string>
#include <utility.hpp>
#include <exception>
#include <functional>
#include <iostream>
#include <srcTypeDefs.hpp>
class srcTypeHandler;
enum {primitive, userdefined};

class TypeDictionary{
	class Context{
		public:
			std::string functionName;
			int ln;
			FunctionVarMap::iterator currentFunc;
			Context(std::string func, unsigned int line, FunctionVarMap::iterator it): functionName(func), ln(line), currentFunc(it){}
			Context():functionName(""), ln(-1){}
	};
	Context current;
	FunctionVarMap fvMap;
	public:
		bool SetContext(std::string fn){
			FunctionVarMap::iterator it = fvMap.find(fn);
			if(it != fvMap.end()){
				current.currentFunc = it;
				current.ln = 0;
				current.functionName = fn;
				return true;
			}
			return false;
		}
		std::pair<bool, VarTypeMap::iterator> Find(std::string funcname, std::string varname){
			FunctionVarMap::iterator fvmIt = fvMap.find(funcname);
			if(fvmIt != fvMap.end()){
				VarTypeMap::iterator vtmIt = fvmIt->second.vtMap.find(varname);
				if(vtmIt != fvmIt->second.vtMap.end()){
					return std::make_pair(true, vtmIt);
				}
			}
			return std::make_pair(false, fvmIt->second.vtMap.end());
			//search for function name and then var name
		}
		std::pair<bool, VarTypeMap::iterator> Find(std::string varname){
			if(current.ln == -1){
				throw std::runtime_error("Context not set"); //for now, std exception
			}else{
				VarTypeMap::iterator it = current.currentFunc->second.vtMap.find(varname);
				if(it != current.currentFunc->second.vtMap.end()){
					return std::make_pair(true, it);
				}
				return std::make_pair(false, current.currentFunc->second.vtMap.end());
			}
		}
		std::pair<bool, VarTypeMap::const_iterator> Find(std::string funcname, std::string varname)const{
			FunctionVarMap::const_iterator fvmIt = fvMap.find(funcname);
			if(fvmIt != fvMap.end()){
				VarTypeMap::const_iterator vtmIt = fvmIt->second.vtMap.find(varname);
				if(vtmIt != fvmIt->second.vtMap.end()){
					return std::make_pair(true, vtmIt);
				}
			}
			return std::make_pair(false, fvmIt->second.vtMap.end());
		}
		std::pair<bool, VarTypeMap::const_iterator> Find(std::string varname)const{
			if(current.ln == -1){
				throw std::runtime_error("Context not set"); //for now, std exception
			}else{
				VarTypeMap::const_iterator it = current.currentFunc->second.vtMap.find(varname);
				if(it != current.currentFunc->second.vtMap.end()){
					return std::make_pair(true, it);
				}
				return std::make_pair(false, current.currentFunc->second.vtMap.end());	
			}
		}
		//output function takes a user-defined output function. I'll provide several defaults for xml/compiled map/whatever else. For now it's pretty dumb.
		void SerializeMap(std::function<std::string(FunctionVarMap)> func){
  			std::string serializedMap = "FunctionVarMap srcTypeMap = "+func(fvMap);
  			OutputHppFile(hppfile, serializedMap);
		}
		friend srcTypeHandler;
};
#endif