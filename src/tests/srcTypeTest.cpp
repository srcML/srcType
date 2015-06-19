#include <srcType.hpp>
#include <iostream>
std::string SerializeToCppUMap(srcTypeNS::FunctionVarMap mp){
    return SerializeMapToString(mp);
}
int main(int argc, char** argv){
	srcTypeNS::srcType typeDict;
	typeDict.ReadArchiveFile(argv[1]);
	//typeDict.SerializeMap(SerializeToCppUMap);
	//std::cerr<<typeDict.size();
}