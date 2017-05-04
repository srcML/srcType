#include <srcType.hpp>
#include <iostream>
#include <srcml.h>
#include <cassert>

/// <summary>
/// Utility function that trims from the right of a string. For now it's just solving a weird issue with srcML
/// and garbage text ending up at the end of the cstring it returns.
/// </summary>
inline char* TrimFromEnd(char *s, size_t len){
    for (int i = len - 1; i > 0; --i){
        if (s[i] != '>'){
            s[i] = 0;
        }else{
            return s;
        }
    }
    return nullptr;
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
    srcml_archive_write_unit(archive, unit);
    
    srcml_unit_free(unit);
    srcml_archive_close(archive);
    srcml_archive_free(archive);
    TrimFromEnd(ch, size);
    return std::string(ch);
}
bool TestPrimitiveTypes(){
    std::string str = "int main(){int c = 5; const int v = 5; static const int e = 5; char array[30];}";
    std::string srcmlStr = StringToSrcML(str);
    srcTypeNS::srcType typeDict(srcmlStr, 0);
    try{
        typeDict.SetContext("testsrcType.cpp", "main");
        {
            auto nameprofile = typeDict.FindVariable("c");
            std::cerr<<"Type1: "<< nameprofile.front().nameoftype<<std::endl;
            assert(nameprofile.front().nameoftype == "int");
            assert(typeDict.IsPrimitive(nameprofile.front().nameoftype) == true);
        }
        {
            auto nameprofile = typeDict.FindVariable("v");
            std::cerr<<"Type2: "<< nameprofile.front().nameoftype<<std::endl;
            assert(nameprofile.front().nameoftype == "int");
            assert(typeDict.IsPrimitive(nameprofile.front().nameoftype) == true);
            assert(nameprofile.front().isConst);
        }
        {
            auto nameprofile = typeDict.FindVariable("e");
            std::cerr<<"Type3: "<< nameprofile.front().nameoftype<<std::endl;
            assert(nameprofile.front().nameoftype == "int"); 
            assert(typeDict.IsPrimitive(nameprofile.front().nameoftype) == true);
            assert(nameprofile.front().isConst);
        }
        {
            auto nameprofile = typeDict.FindVariable("array");
            std::cerr<<"Type4: "<< nameprofile.front().nameoftype<<std::endl;
            assert(nameprofile.front().nameoftype == "char");
            assert(typeDict.IsPrimitive(nameprofile.front().nameoftype) == true);
            //assert(nameprofile.front().isArray);
        }
    }catch(SAXError e){
        std::cerr<<"ERROR: "<<e.message;
    }
    return true;
}

bool TestComplexType(){
    std::string str = "int main(){Object coo = 5; const Object ke_e4e = 5; static const Object caa34 = 5;}";
    std::string srcmlStr = StringToSrcML(str);
    try{
        srcTypeNS::srcType typeDict(srcmlStr, 0);
        typeDict.SetContext("testsrcType.cpp", "main");    
        {
            auto nameprofile = typeDict.FindVariable("coo");
            std::cerr<<"Type1: "<< nameprofile.front().nameoftype<<std::endl;
            assert(nameprofile.front().nameoftype == "Object"
                   && typeDict.IsPrimitive(nameprofile.front().nameoftype) == false);
        }
        {
            auto nameprofile = typeDict.FindVariable("ke_e4e");
            std::cerr<<"Type2: "<< nameprofile.front().nameoftype<<std::endl;
            assert(nameprofile.front().nameoftype == "Object"
                   && typeDict.IsPrimitive(nameprofile.front().nameoftype) == false
                   && nameprofile.front().isConst);
        }
        {
            auto nameprofile = typeDict.FindVariable("caa34");
            std::cerr<<"Type3: "<< nameprofile.front().nameoftype<<std::endl;
            assert(nameprofile.front().nameoftype == "Object"
                   && typeDict.IsPrimitive(nameprofile.front().nameoftype) == false
                   && nameprofile.front().isConst);
        }
    }catch(SAXError e){
        std::cerr<<"ERROR: "<<e.message;
    }
    return true;
}
bool TestPrimitiveTypesMultiDecl(){
    std::string str = "int main(){int c = 5, v = a, e = 5+c;}";
    std::string srcmlStr = StringToSrcML(str);
    
    try{
        srcTypeNS::srcType typeDict(srcmlStr, 0);
        typeDict.SetContext("testsrcType.cpp", "main"); 
        {
            auto nameprofile = typeDict.FindVariable("c");
            std::cerr<<"Type1: "<< nameprofile.front().nameoftype<<std::endl;
            assert(nameprofile.front().nameofidentifier == "c"
                   && nameprofile.front().nameoftype == "int"
                   && typeDict.IsPrimitive(nameprofile.front().nameoftype) == true);
        }
        {
            auto nameprofile = typeDict.FindVariable("v");
            std::cerr<<"Type2: "<< nameprofile.front().nameoftype<<std::endl;
            assert(nameprofile.front().nameofidentifier == "v"
                   && nameprofile.front().nameoftype == "int"
                   && typeDict.IsPrimitive(nameprofile.front().nameoftype) == true);
        }
        {
            auto nameprofile = typeDict.FindVariable("e");
            std::cerr<<"Type3: "<< nameprofile.front().nameoftype <<std::endl;
            assert(nameprofile.front().nameofidentifier == "e"
                   && nameprofile.front().nameoftype == "int"
                   && typeDict.IsPrimitive(nameprofile.front().nameoftype) == true);
        }
    }catch(SAXError e){
        std::cerr<<"ERROR: "<<e.message;
    }
    return true;
}
bool TestNamespacedComplexType(){
    std::string str = "std::string Foo(){std::Object coo = 5; const std::Object ke_e4e = 5; static const std::Object caa34 = 5;}";
    std::string srcmlStr = StringToSrcML(str);
    
    try{
        srcTypeNS::srcType typeDict(srcmlStr, 0);
        typeDict.SetContext("testsrcType.cpp", "Foo"); 
        {
            auto nameprofile = typeDict.FindVariable("coo");
            std::cerr<<"Type1: "<< nameprofile.front().nameoftype<<std::endl;
            assert(nameprofile.front().nameoftype == "Object"
                   && typeDict.IsPrimitive(nameprofile.front().nameoftype) == false
                   && nameprofile.front().namespaces.front() == "std");
        }
        {
            auto nameprofile = typeDict.FindVariable("ke_e4e");
            std::cerr<<"Type2: "<< nameprofile.front().nameoftype<<std::endl;
            assert(nameprofile.front().nameoftype == "Object"
                   && typeDict.IsPrimitive(nameprofile.front().nameoftype) == false
                   && nameprofile.front().isConst
                   && nameprofile.front().namespaces.front() == "std");
        }
        {
            auto nameprofile = typeDict.FindVariable("caa34");
            std::cerr<<"Type3: "<< nameprofile.front().nameoftype<<std::endl;
            assert(nameprofile.front().nameoftype == "Object"
                   && typeDict.IsPrimitive(nameprofile.front().nameoftype) == false
                   && nameprofile.front().isConst
                   && nameprofile.front().namespaces.front() == "std");
        }
    }catch(SAXError e){
        std::cerr<<"ERROR: "<<e.message;
    }
    return true;
}
bool TestFunctionAndReturnTypeID(){
    std::string str = "std::string srcTypeNS::Foo(int a, double b){Object coo = 5; const Object ke_e4e = 5; static const Object caa34 = 5;}";
    std::string srcmlStr = StringToSrcML(str);
    srcTypeNS::srcType typeDict(srcmlStr, 0);
    
    auto findFunc = typeDict.FindFunction("Foo", "intdouble", false);
    if(findFunc != typeDict.data.functionMap.end()){
        FunctionSignaturePolicy::SignatureData functiondata = findFunc->second.front();
        assert(functiondata.name == "Foo");
        assert(functiondata.returnType == "string");
        assert(functiondata.returnTypeNamespaces.front() == "std");
        assert(functiondata.functionNamespaces.front() == "srcTypeNS");
    }else{
        std::cerr<<"Did not find function with name: Foo";
        assert(false);
    }

    return true;
}
bool TestFindFunction(){
    std::string str = "std::string Foo(){std::Object coo = 5; const std::Object ke_e4e = 5; static const std::Object caa34 = 5;}";
    std::string srcmlStr = StringToSrcML(str);
    srcTypeNS::srcType typeDict(srcmlStr, 0);
    
    auto findFunc = typeDict.FindFunction("Foo", "", false);
    if(findFunc != typeDict.data.functionMap.end()){
        FunctionSignaturePolicy::SignatureData functiondata = findFunc->second.front();
        assert(functiondata.name == "Foo");
        assert(functiondata.returnType == "string");
        assert(typeDict.IsPrimitive(functiondata.returnType) == false);
    }else{
        std::cerr<<"Did not find function with name: Foo";
        assert(false);
    }
}
/*
bool TestTypedefedType(){
    return true;
}

bool TestNamespacedTypedefedType(){
    std::string str = "typedef int INTEGER; std::string Foo(){std::Object coo = 5; const std::Object ke_e4e = 5; static const std::Object caa34 = 5;}";
    std::string srcmlStr = StringToSrcML(str);
    try{
        srcTypeNS::srcType typeDict(srcmlStr, 0);
    }catch(SAXError e){
        std::cerr<<"ERROR: "<<e.message;
    }
    return true;
}


*/
int main(int argc, char** argv){
    TestPrimitiveTypes();
    TestComplexType();
    TestNamespacedComplexType();
    //TestNamespacedTypedefedType();
    TestPrimitiveTypesMultiDecl();
    TestFunctionAndReturnTypeID();
    TestFindFunction();
    //srcTypeNS::srcType typeDict;
    //typeDict.ReadArchiveFile(argv[1]);
    
    //typeDict.SerializeMap(SerializeToCppUMap);
    //std::cerr<<typeDict.size();
}