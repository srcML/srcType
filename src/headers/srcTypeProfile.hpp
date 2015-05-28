#ifndef INCLUDED_SRCTYPEPROFILE_HPP
#define INCLUDED_SRCTYPEPROFILE_HPP

#include <unordered_map>
#include <string>
#include <utility.hpp>
struct NameProfile;
struct FunctionProfile;
typedef std::unordered_map<std::string, NameProfile> VarTypeMap;
typedef std::unordered_map<std::string, FunctionProfile> FunctionVarMap;
enum {primitive, userdefined};
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
struct TypeDictionary{
	FunctionVarMap fvMap;
};

#endif