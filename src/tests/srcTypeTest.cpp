/**
 * @file srcTypeTest.cpp
 *
 * @copyright Copyright (C) 2013-2014  SDML (www.srcML.org)
 *
 * The srcML Toolkit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML Toolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <srcType.hpp>
#include <iostream>
#include <srcml.h>
#include <cassert>
#include <stdexcept>
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
void TestPrimitiveTypes(){
    std::string str = "int main(){int c = 5; const int v = 5; static const int e = 5; char array[30];}";
    std::string srcmlStr = StringToSrcML(str);
    srcTypeNS::srcType typeDict(srcmlStr, false);
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
    }catch(std::runtime_error e){
        std::cerr<<e.what();
    }
}

void TestComplexType(){
    std::string str = "int main(){Object coo = 5; const Object ke_e4e = 5; static const Object caa34 = 5;}";
    std::string srcmlStr = StringToSrcML(str);
    try{
        srcTypeNS::srcType typeDict(srcmlStr, false);
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
    }catch(std::runtime_error e){
        std::cerr<<e.what();
    }
}
void TestPrimitiveTypesMultiDecl(){
    std::string str = "int main(){int c = 5, v = a, e = 5+c;}";
    std::string srcmlStr = StringToSrcML(str);
    
    try{
        srcTypeNS::srcType typeDict(srcmlStr, false);
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
    }catch(std::runtime_error e){
        std::cerr<<e.what();
    }
}
void TestNamespacedComplexType(){
    std::string str = "std::string Foo(){std::Object coo = 5; const std::Object ke_e4e = 5; static const std::Object caa34 = 5;}";
    std::string srcmlStr = StringToSrcML(str);
    
    try{
        srcTypeNS::srcType typeDict(srcmlStr, false);
        typeDict.SetContext("testsrcType.cpp", "Foo"); 
        {
            auto nameprofile = typeDict.FindVariable("coo");
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
            auto nameprofile = typeDict.FindVariable("caa34", "Foo", "testsrcType.cpp");
            std::cerr<<"Type3: "<< nameprofile.front().nameoftype<<std::endl;
            assert(nameprofile.front().nameoftype == "Object"
                   && typeDict.IsPrimitive(nameprofile.front().nameoftype) == false
                   && nameprofile.front().isConst
                   && nameprofile.front().namespaces.front() == "std");
        }
    }catch(SAXError e){
        std::cerr<<"ERROR: "<<e.message;
    }catch(std::runtime_error e){
        std::cerr<<e.what();
    }
}
void TestFunctionAndReturnTypeID(){
    std::string str = "std::string srcTypeNS::Foo(int a, double b){Object coo = 5; const Object ke_e4e = 5; static const Object caa34 = 5;}";
    std::string srcmlStr = StringToSrcML(str);
    srcTypeNS::srcType typeDict(srcmlStr, false);
    
    try{
        auto findFunc = typeDict.FindFunction("Foo", 2);
        assert(findFunc.front().name == "Foo");
        assert(findFunc.front().returnType == "string");
        assert(findFunc.front().returnTypeNamespaces.front() == "std");
        assert(findFunc.front().functionNamespaces.front() == "srcTypeNS");
    }catch(std::runtime_error e){
        std::cerr<<e.what();
        assert(false);
    }

}
void TestFindNoArgFunction(){
    std::string str = "std::string Foo(){std::Object coo = 5; const std::Object ke_e4e = 5; static const std::Object caa34 = 5;}";
    std::string srcmlStr = StringToSrcML(str);
    srcTypeNS::srcType typeDict(srcmlStr, false);
    
    try{
        auto findFunc = typeDict.FindFunction("Foo", 0);
        assert(findFunc.front().name == "Foo");
        assert(findFunc.front().returnType == "string");
        assert(typeDict.IsPrimitive(findFunc.front().returnType) == false);
    }catch(std::runtime_error e){
        std::cerr<<"Did not find function with name: Foo";
        assert(false);
    }
}
void TestFindMutliNoArgFunction(){
    std::string str = "std::string Bar(){return \"hi\";} std::string Foo(){return \"hi\";} std::string Foo(){return \"hi\";}";
    std::string srcmlStr = StringToSrcML(str);
    srcTypeNS::srcType typeDict(srcmlStr, false);
    
    try{
        auto findFunc = typeDict.FindFunction("Foo", 0);
        assert(findFunc.size() == 2);
        assert(findFunc.at(0).name == "Foo");
        assert(findFunc.at(0).returnType == "string");
        assert(typeDict.IsPrimitive(findFunc.at(0).returnType) == false);
        assert(findFunc.at(1).name == "Foo");
        assert(findFunc.at(1).returnType == "string");
        assert(typeDict.IsPrimitive(findFunc.at(1).returnType) == false);
    }catch(std::runtime_error e){
        std::cerr<<"Did not find function with name: Foo";
        assert(false);
    }

}
void TestFindMultiArgFunction(){
    std::string str = "std::string Foo(string abc, std::string onetwothree, std::vector<std::string> blee){static const std::Object caa34 = 5;}";
    std::string srcmlStr = StringToSrcML(str);
    srcTypeNS::srcType typeDict(srcmlStr, false);
    try{
        auto findFunc = typeDict.FindFunction("Foo", 3);
        assert(findFunc.front().name == "Foo");
        assert(findFunc.front().returnType == "string");
        assert(typeDict.IsPrimitive(findFunc.front().returnType) == false);
    }catch(std::runtime_error e){
        std::cerr<<e.what();
        assert(false);
    }
}
void TestCollectCallData(){
    try{
        std::string str = "std::string Boo(int a, double b){} std::string Boo(int a, int b){} double Boo(int a, double b, int c){} std::string Foo(int a, double b, std::string c){int c; double d; Boo(c, d, c);}";
        std::string srcmlStr = StringToSrcML(str);
        srcTypeNS::srcType typeDict(srcmlStr, false);
    }catch(std::runtime_error e){
        std::cerr<<e.what();
        assert(false);
    }
}
void TestCollectNestedCallData(){
    std::cerr<<"TEST NESTED CALL"<<std::endl;
    try{
        std::string str = "double Boo(int b){} double Foo(double x, double y){int a; Foo(y, Foo(x, y));}";
        std::string srcmlStr = StringToSrcML(str);
        srcTypeNS::srcType typeDict(srcmlStr, true);
    }catch(std::runtime_error e){
        std::cerr<<e.what();
        assert(false);
    }
}
void TestProbabilityFilter(){
    std::cerr<<"TEST FILTER"<<std::endl;
    try{
        std::string str = "double Boo(int b, double e){} double Boo(double b, double c){} double Foo(double x, double y){int a; Foo(y, Foo(x, Boo(y, x)));}";
        std::string srcmlStr = StringToSrcML(str);
        srcTypeNS::srcType typeDict(srcmlStr, true);
    }catch(std::runtime_error e){
        std::cerr<<e.what();
        assert(false);
    }
}
void TestComputeLiteral(){
    std::cerr<<"TEST LITERAL"<<std::endl;
    try{
        std::string str = "double Boo(int b, std::string e){} double Foo(double x, double y){int a; Foo(y, Foo(5, Boo(y, \"x\")));}";
        std::string srcmlStr = StringToSrcML(str);
        srcTypeNS::srcType typeDict(srcmlStr, true);
    }catch(std::runtime_error e){
        std::cerr<<e.what();
        assert(false);
    }
}
void TestUnresolved(){
    std::cerr<<"TEST UNRESOLVED"<<std::endl;
    try{
        std::string str = "double Boo(int b, std::string e){} double Boo(int b, std::string e, double c){} double Foo(int y){Boo(y, x+y);}";
        std::string srcmlStr = StringToSrcML(str);
        srcTypeNS::srcType typeDict(srcmlStr, true);
    }catch(std::runtime_error e){
        std::cerr<<e.what();
        assert(false);
    }
}
void TestCollectCallDataWithExpr(){
    try{
        std::string str = "std::string Boo(int a, double b){} std::string Foo(int a){int c; double d; Boo(a+c, d);}";
        std::string srcmlStr = StringToSrcML(str);
        srcTypeNS::srcType typeDict(srcmlStr, true);
    }catch(std::runtime_error e){
        std::cerr<<e.what();
        assert(false);
    }
}
/*
void TestTypedefedType(){
}

void TestNamespacedTypedefedType(){
    std::string str = "typedef int INTEGER; std::string Foo(){std::Object coo = 5; const std::Object ke_e4e = 5; static const std::Object caa34 = 5;}";
    std::string srcmlStr = StringToSrcML(str);
    try{
        srcTypeNS::srcType typeDict(srcmlStr);
    }catch(SAXError e){
        std::cerr<<"ERROR: "<<e.message;
    }
}


*/
int main(int argc, char** argv){
    TestPrimitiveTypes();
    TestComplexType();
    TestNamespacedComplexType();
    TestPrimitiveTypesMultiDecl();
    TestFunctionAndReturnTypeID();
    TestFindNoArgFunction();
    TestFindMultiArgFunction();
    TestFindMutliNoArgFunction();
    TestCollectCallData();
    TestCollectNestedCallData();
    TestProbabilityFilter();
    TestComputeLiteral();
    TestUnresolved();
    //TestNamespacedTypedefedType();
    //srcTypeNS::srcType typeDict;
    //typeDict.ReadArchiveFile(argv[1]);
    
    //typeDict.SerializeMap(SerializeToCppUMap);
    //std::cerr<<typeDict.size();
}