#include <string>
#include <srcType.hpp>
#include <cassert>
#include <iostream>

int main(int argc, char** argv){
	srcTypeNS::srcType typeDict;
	typeDict.ReadArchiveFile(argv[1]);
	typeDict.SetFunctionContext("code.cpp", 1);
	typeDict.SetVariableContext("Foo",1);
	{
		auto nameprofile = typeDict.FindVariable("coo").second;
		std::cerr<<"Name: "<<nameprofile.name<<" Type: "<< nameprofile.type<<std::endl;
		assert(nameprofile.type == "Object" && nameprofile.isPrimitive == false && nameprofile.varNamespace == "std");
	}
	{
		auto nameprofile = typeDict.FindVariable("ke_e4e").second;
		std::cerr<<"Name: "<<nameprofile.name<<" Type: "<< nameprofile.type<<std::endl;
	    assert(nameprofile.type == "Object" && nameprofile.isPrimitive == false && nameprofile.isConst && nameprofile.varNamespace == "std");
	}
	{
		auto nameprofile = typeDict.FindVariable("caa34").second;
		std::cerr<<"Name: "<<nameprofile.name<<" Type: "<< nameprofile.type<<std::endl;
		assert(nameprofile.type == "Object" && nameprofile.isPrimitive == false && nameprofile.isConst && nameprofile.varNamespace == "std");
	}
}