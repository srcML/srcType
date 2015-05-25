#ifndef INCLUDED_SRCTYPEPROFILE_HPP
#define INCLUDED_SRCTYPEPROFILE_HPP

#include <unordered_map>
#include <string>
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
};
struct FunctionProfile{
		std::string name;
		std::string fnNamespace;
		std::string returnType;
		std::string returnTypeNamespace;
		VarTypeMap vtMap;
};
struct TypeDictionary{
	FunctionVarMap fvMap;
};

#endif