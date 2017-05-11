#include <string>
#include <srcType.hpp>
#include <cassert>
#include <iostream>

int main(int argc, char** argv){
	srcTypeNS::srcType typeDict;
	typeDict.ReadArchiveFile(argv[1]);
	typeDict.SetContext("code.cpp", "Foo");
	{
		auto nameprofile = typeDict.FindVariable("coo");
		std::cerr<<"Name: "<<nameprofile.front().nameofidentifier<<" Type: "<< nameprofile.front().nameoftype<<std::endl;
		assert(nameprofile.front().nameoftype == "Object" 
				&& typeDict.IsPrimitive(nameprofile.front().nameoftype) == false 
				&& nameprofile.front().namespaces.front() == "std");
	}
	{
		auto nameprofile = typeDict.FindVariable("ke_e4e");
		std::cerr<<"Name: "<<nameprofile.front().nameofidentifier<<" Type: "<< nameprofile.front().nameoftype<<std::endl;
	    assert(nameprofile.front().nameoftype == "Object" && typeDict.IsPrimitive(nameprofile.front().nameoftype) == false && nameprofile.front().isConst && nameprofile.front().namespaces.front() == "std");
	}
	{
		auto nameprofile = typeDict.FindVariable("caa34");
		std::cerr<<"Name: "<<nameprofile.front().nameofidentifier<<" Type: "<< nameprofile.front().nameoftype<<std::endl;
		assert(nameprofile.front().nameoftype == "Object" && typeDict.IsPrimitive(nameprofile.front().nameoftype) == false && nameprofile.front().isConst && nameprofile.front().namespaces.front() == "std");
	}
}