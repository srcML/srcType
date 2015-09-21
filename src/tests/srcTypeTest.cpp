#include <srcType.hpp>
#include <iostream>
#include <srcml/srcml.h>
std::string SerializeToCppUMap(srcTypeNS::FunctionVarMap mp){
    return SerializeMapToString(mp);
}
std::string StringToSrcML(std::string str){
	struct srcml_archive* archive;
	struct srcml_unit* unit;
	size_t size = 0;

	char *ch = new char[str.size()];

	archive = srcml_archive_create();
	srcml_archive_enable_option(archive, SRCML_OPTION_POSITION);
	srcml_archive_write_open_memory(archive, &ch, &size);

	unit = srcml_unit_create(archive);
	srcml_unit_set_language(unit, SRCML_LANGUAGE_CXX);
	srcml_unit_set_filename(unit, "testsrcType.cpp");

	srcml_unit_parse_memory(unit, str.c_str(), str.size());
	srcml_write_unit(archive, unit);
	
	srcml_unit_free(unit);
	srcml_archive_close(archive);
	srcml_archive_free(archive);

	return std::string(ch);
}
bool TestPrimitiveTypes(){
	std::string str = "int main(){int c = 5; const int v = 5; static const int e = 5;}";
	std::string srcmlStr = StringToSrcML(str);
	srcTypeNS::srcType typeDict(srcmlStr, 0);
	typeDict.SetContext("main",1);
	std::cerr<<srcmlStr;
	{
		auto nameprofile = typeDict.Find("c").second;
		std::cerr<<"Type1: "<< nameprofile.type<<std::endl;
		if(nameprofile.type != "int" || nameprofile.category != srcTypeNS::VarCategory::primitive){
			return false;
		}
	}
	{
		auto nameprofile = typeDict.Find("v").second;
		std::cerr<<"Type2: "<< nameprofile.type<<std::endl;
		if(nameprofile.type != "int" || nameprofile.category != srcTypeNS::VarCategory::primitive || nameprofile.isConst != true){
			return false;
		}
	}
	{
		auto nameprofile = typeDict.Find("e").second;
		std::cerr<<"Type3: "<< nameprofile.type<<std::endl;
		if(nameprofile.type != "int" || nameprofile.category != srcTypeNS::VarCategory::primitive || nameprofile.isConst != true){
			return false;
		}
	}
	return true;
}
bool TestComplexType(){
	return true;
}
bool TestTypedefedType(){
	return true;
}
bool TestFunctionReturnType(){
	return true;
}
bool TestNamespacedTypedefedType(){
	return true;
}
bool TestNamespacedComplexType(){
	return true;
}
int main(int argc, char** argv){
	TestPrimitiveTypes();
	//srcTypeNS::srcType typeDict;
	//typeDict.ReadArchiveFile(argv[1]);
	//typeDict.SerializeMap(SerializeToCppUMap);
	//std::cerr<<typeDict.size();
}