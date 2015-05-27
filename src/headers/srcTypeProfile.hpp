#ifndef INCLUDED_SRCTYPEPROFILE_HPP
#define INCLUDED_SRCTYPEPROFILE_HPP

#include <unordered_map>
#include <string>
#include <utility.hpp>
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
		void clear(){
			linenumber = 0;
			category = 0;
			type.clear();
			name.clear();
			namespacename.clear();
			classMember = false;
		}
};
struct FunctionProfile{
		std::string name;
		std::string fnNamespace;
		std::string returnType;
		std::string returnTypeNamespace;
		VarTypeMap vtMap;
		void clear(){
			returnType.clear();
			name.clear();
			fnNamespace.clear();
			returnTypeNamespace.clear();
			vtMap.clear();
		}
};
struct TypeDictionary{
	FunctionVarMap fvMap;
};

#endif